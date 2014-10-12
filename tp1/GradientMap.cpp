#include <opencv2/highgui/highgui.hpp>
#include "GradientMap.h"
#include "Gradient.h"

GradientMap::GradientMap(int rows, int cols)
{
    resize(rows, cols);
}
GradientMap::GradientMap(const Image &image, const GradientKernel &kernel)
{
    buildFromImage(image, kernel);
}

int GradientMap::width() const
{
    if (height() > 0)
        return _gradients[0].size();
    return 0;
}
int GradientMap::height() const
{
    return _gradients.size();
}

const Gradient &GradientMap::getGradientAt(int row, int col) const
{
    return _gradients[row][col];
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
    _gradients.resize(rows);
    for (int i = 0; i < rows; ++i)
        _gradients[i].resize(cols);
}

Gradient GradientMap::computeGradientAt(cv::Mat grayscaleMat, int row, int col, const GradientKernel &kernel)
{
    Gradient grad;
    MaskVec masks;
    int maskCount;

    masks = kernel.masks();
    maskCount = masks.size();

    grad.resize(maskCount);

    // ToDo JRA: GradientMap::computeGradientAt(): Fixer une règle pour le calcul des gradients sur les bordures
    if (row == 0 || row == grayscaleMat.rows - 1 || col == 0 || col == grayscaleMat.cols - 1)
    {
        for (int i = 0; i < maskCount; ++i)
            grad.setValueAt(i, 0);
        return grad;
    }
    //////////////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < maskCount; ++i)
        grad.setValueAt(i, computeGradientValueAt(grayscaleMat, row, col, masks[i]));

    return grad;
}
double GradientMap::computeGradientValueAt(cv::Mat grayscaleMat, int row, int col, const Mask &mask)
{
    uchar intensity;
    double gradientValue;

    gradientValue = 0;

    // ToDo JRA: GradientMap::computeGradientValueAt(): Déterminer dynamiquement les intervalles [-i,i] [-j,j]

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

//double GradientMap::valeurGradientPixel(cv::Mat picture, int rows, int cols, Mask mask)
//{
//cv::Scalar intensity;
//double valeurGradient;

//    for(int i = 0; i < 2; i++)
//    {
//        for(int j = 0; j < 2; j++ )
//        {
//            intensity = picture.at<uchar>(rows-i, cols-j);
//            valeurGradient += intensity.val[0] * mask[i][j];
//        }
//    }
//return valeurGradient;
//}

//int valeurGradientPixel(cv::Mat image, int y, int x, int mask[][])
//{
//    cv::Scalar intensity;
//    int gradien;
//    //I'(x,y) = SS I(y-1, x-1)*mash (y,x)
//    for(int i=-1; i < 1; i++)
//    {
//        for(int j=-1; j < 1; j++)
//        {
//            intensity = image.at<uchar>(y-j,x-i)
//            gradien += intensity.val[0] * mask[i][j];
//        }
//    }
//    return gradien;
//}

//void caclulConv2D (cv::Mat picture , Mask maskX, Mask maskY)//maskX=MaskVec[0]
//{
//    for(int i = 1; i < picture.rows;)
//    {
//        for()
//        {

//        }
//    }
//}

//void calculConv2D ( cv::Mat image, int maskY[][], int maskX[][] )
//{
//    std::vector < vector <gradient> > gradienMap2D; // en doit difinire gradient
//    // je trouve que ça sera mieux de déclarer gradienMap comme attribut
//    for(int i=1; i<image.rows; i++ )
//    {
//        gradienMap2D.push_back( std::vector <gradient> (image.cols));
//        for(int j=1; j<image.cols)
//        {
//           gradienMap2D[i][j][0] = valeurGradientPixel(image, i,j,maskY);
//           gradienMap2D[i][j][1] = valeurGradientPixel(image, i,j,maskX);
//        }
//    }

//}

//void calculConv4D ( cv::Mat Image, int maskY[][], int maskZ[][], int maskX[][], int maskW[][])
//{
//    std::vector < vector <gradient> > gradienMap4D; // en doit difinire gradient
//    // je trouve que ça sera mieux de déclarer gradienMap comme attribut
//    for(int i=1; i<image.rows; i++ )
//    {
//        gradienMap4D.push_back( std::vector <gradient> (image.cols));
//        for(int j=1; j<image.cols)
//        {
//           gradienMap4D[i][j][0] = valeurGradientPixel(image, i,j,maskY);
//           gradienMap4D[i][j][1] = valeurGradientPixel(image, i,j,maskZ);
//           gradienMap4D[i][j][2] = valeurGradientPixel(image, i,j,maskX);
//           gradienMap4D[i][j][3] = valeurGradientPixel(image, i,j,maskW);
//        }
//    }


//}
