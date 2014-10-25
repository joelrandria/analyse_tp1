#include "EdgeDetectionWindow.h"
#include "ui_EdgeDetectionWindow.h"

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
    _gradientMapMax(_image, _kernel, true),//true pr dir normalise
    _hysterisisLowThreshold((hysterisisLowThreshold*255)/100),
    _hysterisisHighThreshold((hysterisisHighThreshold*255)/100)

{
    ui->setupUi(this);

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

    _gradientMapMax.seuillageHyest(_hysterisisHighThreshold,_hysterisisLowThreshold);
    //_gradientMapMax.sauveGradient("/home/meguehout/AnalyseTp1/analyse_tp1/tp1/Test2.txt");
    _gradientMapMax.affinage();
    Composant composantGradient;

    QImage* pImage = new QImage(mapWidth, mapHeight, QImage::Format_RGB888);

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            composantGradient = _gradientMapMax.getComposantAt(row, col);

            uint pixelColor;
            switch (composantGradient.dir)
            {
            case 0: // Direction X => Rouge

                pixelColor = qRgb(composantGradient.valS, 0, 0);
                break;

            case 1: // Direction Y => Vert

                pixelColor = qRgb(0, composantGradient.valS, 0);
                break;
             /********4D**************/
            case 2: // Direction YX => Bleu

                pixelColor = qRgb(0, 0,composantGradient.valS);
                break;

            case 3: // Direction X-Y => yellow

                pixelColor = qRgb(composantGradient.valS, composantGradient.valS, 0);
                break;

            }//AJOUTER DES TESTE PR LES AUTRE DIRECTION

            pImage->setPixel(col, row, pixelColor);
        }
    }

    ui->pixmapLabel->setPixmap(QPixmap::fromImage(*pImage));
}
