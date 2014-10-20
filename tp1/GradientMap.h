#ifndef GRADIENTMAP_H
#define GRADIENTMAP_H

#include <vector>

#include "Mask.h"
#include "Image.h"
#include "Gradient.h"
#include "GradientKernel.h"

class GradientMap
{
protected:

    std::vector<std::vector<Gradient> > _gradients;

    int _rows;/**/
    int _cols;/**/

public:
//    pr les création de notre tableau gradient soit on donne "rows" "cols"
//    et ça crée un tableau de gradient vide
//    OU l'image' est le 'kernel' est ça crée un tab gradient avec valeur
    GradientMap(int rows, int cols);
    GradientMap(const Image& image, const GradientKernel& kernel);

    int width() const;
    int height() const;

    const Gradient& getGradientAt(int row, int col) const;


private:

    void resize(int rows, int cols);//resrvation d'espace
    void buildFromImage(const Image &image, const GradientKernel &kernel);

    static Gradient computeGradientAt(cv::Mat grayscaleMat, int row, int col, const GradientKernel& kernel);
    static double computeGradientValueAt(cv::Mat grayscaleMat, int row, int col, const Mask& mask);

};

#endif // GRADIENTMAP_H
