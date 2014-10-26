#include "GradientMap.h"
#include "Gradient.h"

#include <QElapsedTimer>
#include <QTime>
#include <QDebug>

#include <opencv2/highgui/highgui.hpp>

GradientMap::GradientMap(int rows, int cols)
{
    resize(rows, cols); // crée gradientMap vide juste reservation
}
GradientMap::GradientMap(const Image &image, const GradientKernel &kernel)
{
    buildFromImage(image, kernel);//creé un gradientMap avec les valeur du grand en
                                  //appliquant le kernel sur l'image
}

int GradientMap::width() const
{
    if (height() > 0)
        return _gradients[0].size();//nbr colone du tableau du gradient
    return 0;
}
int GradientMap::height() const
{
    return _gradients.size();//nbt ligne du tableau du gradient
}

const Gradient &GradientMap::getGradientAt(int row, int col) const
{
    return _gradients[row][col];//return un vecteur de gradient d'une position de gradienMap
}

void GradientMap::buildFromImage(const Image &image, const GradientKernel& kernel)
{
    cv::Mat grayscaleMat = image.grayscaleMatrix();

    resize(grayscaleMat.rows, grayscaleMat.cols);

    for (int row = 0; row < grayscaleMat.rows; ++row)
        for (int col = 0; col < grayscaleMat.cols; ++col)
            _gradients[row][col] = computeGradientAt(grayscaleMat, row, col, kernel);
}

void GradientMap::resize(int rows, int cols)
{
    _rows = rows;
    _cols = cols;

    _gradients.resize(rows);
    for (int i = 0; i < rows; ++i)
        _gradients[i].resize(cols);
}

Gradient GradientMap::computeGradientAt(cv::Mat &grayscaleMat, int row, int col, const GradientKernel &kernel)
{
    MaskVec masks = kernel.masks();
    int maskCount = masks.size();

    Gradient grad(maskCount);

    if (row == 0 || row == grayscaleMat.rows - 1 || col == 0 || col == grayscaleMat.cols - 1)
    {
        for (int i = 0; i < maskCount; ++i)
            grad.setValueAt(i, 0);
        return grad;
    }

    for (int i = 0; i < maskCount; ++i)
        grad.setValueAt(i, computeGradientValueAt(grayscaleMat, row, col, masks[i]));

    return grad;
}
double GradientMap::computeGradientValueAt(cv::Mat& grayscaleMat, int row, int col, const Mask &mask)
{
    uchar intensity;
    double gradientValue;

    gradientValue = 0;

    for(int i = -1; i <= 1; ++i) // itération sur les colonnes de l'image et du masque
    {
        for(int j = -1; j <= 1; ++j) // itération sur les lignes de l'image et du masque
        {
            intensity = grayscaleMat.at<uchar>(row - j, col - i);
            gradientValue += intensity * mask[j + 1][i + 1];
        }
    }

    return gradientValue;
}
