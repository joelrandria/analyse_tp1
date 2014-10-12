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
    GradientMap(const Image& image, const GradientKernel& kernel);

    int width() const;
    int height() const;

    const Gradient& getGradientAt(int row, int col) const;

private:

    void resize(int rows, int cols);
    void buildFromImage(const Image &image, const GradientKernel &kernel);

    static Gradient computeGradientAt(cv::Mat grayscaleMat, int row, int col, const GradientKernel& kernel);
    static double computeGradientValueAt(cv::Mat grayscaleMat, int row, int col, const Mask& mask);

};

#endif // GRADIENTMAP_H
