#include "InteractiveEdgeDetectionDialog.h"
#include "ui_InteractiveEdgeDetectionDialog.h"

#include <QFileDialog>
#include <QPainter>
#include <QList>
#include <QLine>

#include "ConnectedComponent.h"

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

            if(ui->edgeClosureGroupBox->isChecked())
            {
                if (ui->gradientEdgeClosureRadioButton->isChecked())
                    _gradientMapMax.fermetureDirectionGradient();
                else
                    _gradientMapMax.fermetureDirectionContour();

                _gradientMapMax.affinageV4();

                ConnectedComponent::fromGradientMapMax(_gradientMapMax, _connectedComponents);
            }
        }

    }

    int mapWidth = _gradientMapMax.width();
    int mapHeight = _gradientMapMax.height();

    QImage* pImage = new QImage(mapWidth, mapHeight, QImage::Format_RGB888);

    if (!ui->displayModelOnlyCheckBox->isChecked())
    {
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
        ConnectedComponent component;
        for (int i = 0; i < _connectedComponents.size(); ++i)
        {
            component = _connectedComponents[i];
            for (int j = 0; j < component.ends().size(); ++j)
            {
                QPoint end = component.ends()[j];
                pImage->setPixel(end, qRgb(255, 255, 255));
            }
        }
    }
    else
    {
//        int i;

        // Affichage du modèle de contour (segments)
//        QList<QLine> edgeSegments;

//        for (i = 0; i < _connectedComponents.size(); ++i)
//            edgeSegments += _connectedComponents[i].segments();

//        QPainter painter(pImage);

//        for (i = 0; i < edgeSegments.size(); ++i)
//            painter.drawLine(edgeSegments[i]);

        //////////////////////// ToDo: Virer ce test ////////////////////////

        QPainter painter(pImage);
        painter.fillRect(0, 0, pImage->width(), pImage->height(), qRgb(0, 0, 0));

        ConnectedComponent testComponent;
        for (int i = 0; i < _connectedComponents.size(); ++i)
            for (int j = 0; j < _connectedComponents[i].ends().size(); ++j)
                if (_connectedComponents[i].ends()[j] == QPoint(224, 173))
                    testComponent = _connectedComponents[i];

        painter.setPen(qRgb(255, 255, 255));

        QList<QList<QPoint> > testSubComponents = testComponent.subComponents();
        for (int j = 0; j < testSubComponents.size(); ++j)
            for (int k = 0; k < testSubComponents[j].size(); ++k)
                painter.drawPoint(testSubComponents[j][k]);

        /////////////////////////////////////////////////////////////////////
    }

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
}
void InteractiveEdgeDetectionDialog::on_edgeRefiningCheckbox_toggled(bool arg1)
{
    enableEdgeClosure(arg1);

    updateView();
}

void InteractiveEdgeDetectionDialog::enableEdgeClosure(bool enabled)
{
    ui->edgeClosureGroupBox->setEnabled(enabled);
    if (!enabled)
        ui->edgeClosureGroupBox->setChecked(false);

    enableModelDisplay(enabled);
}
void InteractiveEdgeDetectionDialog::on_edgeClosureGroupBox_toggled(bool)
{
    updateView();
}
void InteractiveEdgeDetectionDialog::on_gradientEdgeClosureRadioButton_toggled(bool)
{
    updateView();
}
void InteractiveEdgeDetectionDialog::on_directionEdgeClosureRadioButton_toggled(bool)
{
    updateView();
}

void InteractiveEdgeDetectionDialog::enableModelDisplay(bool enable)
{
    ui->displayModelOnlyCheckBox->setEnabled(enable);
    if (!enable)
        ui->displayModelOnlyCheckBox->setChecked(false);
}
void InteractiveEdgeDetectionDialog::on_displayModelOnlyCheckBox_toggled(bool)
{
    updateView();
}
