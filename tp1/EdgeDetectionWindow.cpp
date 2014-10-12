#include "EdgeDetectionWindow.h"
#include "ui_EdgeDetectionWindow.h"

#include <cmath>

EdgeDetectionWindow::EdgeDetectionWindow(const Image& image,
                                         const GradientKernel& kernel,
                                         QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::EdgeDetectionWindow),
    _image(image),
    _kernel(kernel),
    _gradientMap(_image, _kernel)
{
    ui->setupUi(this);

    updateView();
}
EdgeDetectionWindow::~EdgeDetectionWindow()
{
    delete ui;
}

void EdgeDetectionWindow::updateView() const
{
    int mapWidth = _gradientMap.width();
    int mapHeight = _gradientMap.height();
    float normalizationFactor = _kernel.getNormalizationFactor();

    QImage* pImage = new QImage(mapWidth, mapHeight, QImage::Format_RGB888);

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int col = 0; col < mapWidth; ++col)
        {
            Gradient gradient = _gradientMap.getGradientAt(row, col);

            int maxValueIndex = gradient.getMaxValueDirection();
            int normalizedMaxValue = abs(gradient.getValueAt(maxValueIndex)) * normalizationFactor;

            uint pixelColor;
            switch (maxValueIndex)
            {
            case 0: // Direction X => Rouge

                pixelColor = qRgb(normalizedMaxValue, 0, 0);
                break;

            case 1: // Direction Y => Vert

                pixelColor = qRgb(0, normalizedMaxValue, 0);
                break;
            }

            pImage->setPixel(col, row, pixelColor);
        }
    }

    ui->pixmapLabel->setPixmap(QPixmap::fromImage(*pImage));
}
