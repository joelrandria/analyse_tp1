#include "EdgeDetectionWindow.h"
#include "ui_EdgeDetectionWindow.h"

#include <QFileDialog>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

EdgeDetectionWindow::EdgeDetectionWindow(const Image& image,
                                         const GradientKernel& kernel,
                                         int hysterisisLowThreshold,
                                         int hysterisisHighThreshold,
                                         QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::EdgeDetectionWindow),
    _image(image),
    _kernel(kernel),
    _gradientMapMax(_image, _kernel, true),
    _hysterisisLowThreshold((hysterisisLowThreshold*255)/100),
    _hysterisisHighThreshold((hysterisisHighThreshold*255)/100)
{
    ui->setupUi(this);

    _gradientMapMax.seuillageHyest(_hysterisisHighThreshold,_hysterisisLowThreshold);
    _gradientMapMax.affinage();

    ConnectedComponent::fromGradientMapMax(_gradientMapMax, _connectedComponents);

    qDebug() << "Nombre de composantes connexes trouvées: " << _connectedComponents.size();

    updateView();
}
EdgeDetectionWindow::~EdgeDetectionWindow()
{
    delete ui;
}

void EdgeDetectionWindow::updateView()
{
    int mapWidth = _gradientMapMax.width();
    int mapHeight = _gradientMapMax.height();

    PixelGradientInfo composantGradient;

    QImage* pImage = new QImage(mapWidth, mapHeight, QImage::Format_RGB888);

    // Affichage des pixels filtrés
    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            composantGradient = _gradientMapMax.composantAt(row, col);

            uint pixelColor = 0;

            switch (composantGradient.dir)
            {
            case 0: // Direction X => Rouge

                pixelColor = qRgb(composantGradient.valS, 0, 0);
                break;

            case 1: // Direction Y => Vert

                pixelColor = qRgb(0, composantGradient.valS, 0);
                break;

            case 2: // Direction YX => Bleu

                pixelColor = qRgb(0, 0,composantGradient.valS);
                break;

            case 3: // Direction X-Y => Jaune

                pixelColor = qRgb(composantGradient.valS, composantGradient.valS, 0);
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

    ui->pixmapLabel->setPixmap(QPixmap::fromImage(*pImage));
}

void EdgeDetectionWindow::on_actionEnregistrer_triggered()
{
    QString savePath = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer sous",
                                                    "",
                                                    tr("Fichiers images (*.png *.jpg *.bmp)"));
    if (!savePath.isEmpty())
        ui->pixmapLabel->pixmap()->save(savePath);
}
