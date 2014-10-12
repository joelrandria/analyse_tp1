#ifndef GRADIENTMAP_H
#define GRADIENTMAP_H

#include <vector>

#include "Mask.h"
#include "Image.h"
#include "Gradient.h"
#include "GradientKernel.h"

class GradientMap
{
private:

    std::vector<std::vector<Gradient> > _gradients;

public:

    GradientMap(int rows, int cols);

    static GradientMap fromImage(const Image& image, const GradientKernel& kernel);

private:

    static Gradient gradientAt(cv::Mat grayscaleMat, int row, int col, const GradientKernel& kernel);
    static double gradientValueAt(cv::Mat grayscaleMat, int row, int col, const Mask& mask);

};

#endif // GRADIENTMAP_H
