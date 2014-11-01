#include "InteractiveEdgeDetectionDialog.h"
#include "ui_InteractiveEdgeDetectionDialog.h"

#include <QFileDialog>

InteractiveEdgeDetectionDialog::InteractiveEdgeDetectionDialog(const Image& image,
                                                               QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::InteractiveEdgeDetectionDialog),
    _image(image),
    _gradientMapMax(image, GradientKernel::Prewitt2D(), true)
{
    ui->setupUi(this);

    updateGradientMapMax();
}
InteractiveEdgeDetectionDialog::~InteractiveEdgeDetectionDialog()
{
    delete ui;
}

void InteractiveEdgeDetectionDialog::updateGradientMapMax()
{
    _gradientMapMax = GradientMapMax(_image, selectedKernel(), true);

    updateView();
}

void InteractiveEdgeDetectionDialog::updateView()
{
    bool bDrawRawValues = true;
    if (ui->hysteresisThresholdingGroupBox->isChecked())
    {
        bDrawRawValues = false;

        _gradientMapMax.seuillageHyest(hysterisisHighThreshold(), hysterisisLowThreshold());

        if (ui->edgeRefiningCheckbox->isChecked())
        {
            _gradientMapMax.affinage();

            ConnectedComponent::fromGradientMapMax(_gradientMapMax, _connectedComponents);

            if(ui->edgeClosureCheckbox->isChecked())
            {
                _gradientMapMax.fermeture();
            }
        }

    }

    int mapWidth = _gradientMapMax.width();
    int mapHeight = _gradientMapMax.height();

    QImage* pImage = new QImage(mapWidth, mapHeight, QImage::Format_RGB888);

    // Affichage des pixels filtrés
    PixelGradientInfo composantGradient;
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            composantGradient = _gradientMapMax.composantAt(row, col);

            uint pixelColor = 0;
            switch (composantGradient.dir)
            {
            case 0: // Direction X => Rouge

                pixelColor = qRgb(bDrawRawValues ? composantGradient.val : composantGradient.valS, 0, 0);

                break;

            case 1: // Direction Y => Vert

                pixelColor = qRgb(0, bDrawRawValues ? composantGradient.val : composantGradient.valS, 0);

                break;

            case 2: // Direction YX => Bleu

                pixelColor = qRgb(0, 0, bDrawRawValues ? composantGradient.val : composantGradient.valS);

                break;

            case 3: // Direction X-Y => Jaune

                pixelColor = qRgb(bDrawRawValues ? composantGradient.val : composantGradient.valS, bDrawRawValues ? composantGradient.val : composantGradient.valS, 0);

                break;
            }

            pImage->setPixel(col, row, pixelColor);
        }
    }

    // Affichage des extrémités des composantes connexes en blanc
//    ConnectedComponent component;
//    for (int i = 0; i < _connectedComponents.size(); ++i)
//    {
//        component = _connectedComponents[i];
//        for (int j = 0; j < component.ends().size(); ++j)
//        {
//            QPoint end = component.ends()[j];
//            pImage->setPixel(end, qRgb(255, 255, 255));
//        }
//    }

    ui->pixmapLabel->setPixmap(QPixmap::fromImage(*pImage));
}

GradientKernel InteractiveEdgeDetectionDialog::selectedKernel() const
{
    GradientKernel gk;

    if (ui->filteringTypeComboBox->currentText().contains("Bi-directionnel"))
    {
        if(ui->maskTypeComboBox->currentText().contains("Prewitt"))
        {
            gk = GradientKernel::Prewitt2D();
        }
        else if(ui->maskTypeComboBox->currentText().contains("Sobel"))
        {
            gk = GradientKernel::Sobel2D();
        }
        else if (ui->maskTypeComboBox->currentText().contains("Kirsch"))
        {
            gk = GradientKernel::Kirsch2D();
        }
    }
    else if (ui->filteringTypeComboBox->currentText().contains("Multi-directionnel"))
    {
        if(ui->maskTypeComboBox->currentText().contains("Prewitt"))
        {
            gk = GradientKernel::Prewitt4D();
        }
        else if(ui->maskTypeComboBox->currentText().contains("Sobel"))
        {
            gk = GradientKernel::Sobel4D();
        }
        else if (ui->maskTypeComboBox->currentText().contains("Kirsch"))
        {
            gk = GradientKernel::Kirsch4D();
        }
    }

    return gk;
}

int InteractiveEdgeDetectionDialog::hysterisisLowThreshold() const
{
    return (ui->lowThresholdLineEdit->value() * 255) / 100;
}
int InteractiveEdgeDetectionDialog::hysterisisHighThreshold() const
{
    return (ui->highThresholdLineEdit->value() * 255) / 100;
}

void InteractiveEdgeDetectionDialog::setHysterisisDefaultValues()
{
    ui->lowThresholdLineEdit->setValue(10);
    ui->highThresholdLineEdit->setValue(20);
}

void InteractiveEdgeDetectionDialog::on_actionEnregistrer_sous_triggered()
{
    QString savePath = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer sous",
                                                    "",
                                                    tr("Fichiers images (*.png *.jpg *.bmp)"));
    if (!savePath.isEmpty())
        ui->pixmapLabel->pixmap()->save(savePath);
}

void InteractiveEdgeDetectionDialog::on_maskTypeComboBox_currentIndexChanged(int)
{
    setHysterisisDefaultValues();

    updateGradientMapMax();
}
void InteractiveEdgeDetectionDialog::on_filteringTypeComboBox_currentIndexChanged(int)
{
    setHysterisisDefaultValues();

    updateGradientMapMax();
}

void InteractiveEdgeDetectionDialog::on_hysteresisThresholdingGroupBox_toggled(bool arg1)
{
    enableEdgeRefining(arg1);
    enableEdgeClosure(arg1);

    updateView();
}

void InteractiveEdgeDetectionDialog::on_highThresholdSlider_valueChanged(int)
{
    updateView();
}
void InteractiveEdgeDetectionDialog::on_highThresholdLineEdit_valueChanged(int arg1)
{
    if (ui->lowThresholdSlider->value() > arg1)
        ui->lowThresholdSlider->setValue(arg1);

    updateView();
}
void InteractiveEdgeDetectionDialog::on_lowThresholdSlider_valueChanged(int)
{
    updateView();
}
void InteractiveEdgeDetectionDialog::on_lowThresholdLineEdit_valueChanged(int arg1)
{
    if (ui->highThresholdSlider->value() < arg1)
        ui->highThresholdSlider->setValue(arg1);

    updateView();
}

void InteractiveEdgeDetectionDialog::enableEdgeRefining(bool enabled)
{
    ui->edgeRefiningCheckbox->setEnabled(enabled);
    if (!enabled)
        ui->edgeRefiningCheckbox->setChecked(false);
}
void InteractiveEdgeDetectionDialog::on_edgeRefiningCheckbox_toggled(bool)
{
    updateView();
}

void InteractiveEdgeDetectionDialog::enableEdgeClosure(bool enabled)
{
    ui->edgeClosureCheckbox->setEnabled(enabled);
    if (!enabled)
        ui->edgeClosureCheckbox->setChecked(false);
}
void InteractiveEdgeDetectionDialog::on_edgeClosureCheckbox_toggled(bool)
{
    updateView();
}
