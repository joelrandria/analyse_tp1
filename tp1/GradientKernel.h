#ifndef GRADIENTKERNEL_H
#define GRADIENTKERNEL_H

#include <vector>
#include <opencv2/core/core.hpp>

#include "Mask.h"

class GradientKernel
{
private:

    MaskVec _masks;
    float _normalizationFactor;

    static GradientKernel* _prewitt2d;// est on ne peux pas utilise la meme valeur pr 2D et 4D??
    static GradientKernel* _sobel2d;
    static GradientKernel* _kirsch2d;

    static GradientKernel* _prewitt4d;
    static GradientKernel* _sobel4d;
    static GradientKernel* _kirsch4d;

public:

    const MaskVec& masks() const { return _masks; }
    float getNormalizationFactor() const { return _normalizationFactor; }

    static const GradientKernel& Prewitt2D();
    static const GradientKernel& Sobel2D();
    static const GradientKernel& Kirsch2D();

    static const GradientKernel& Prewitt4D();
    static const GradientKernel& Sobel4D();
    static const GradientKernel& Kirsch4D();

};

#endif // GRADIENTKERNEL_H
