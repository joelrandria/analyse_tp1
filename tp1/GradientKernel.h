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

    static GradientKernel* _prewitt2d;

public:

    const MaskVec& masks() const { return _masks; }
    float getNormalizationFactor() const { return _normalizationFactor; }

    static const GradientKernel& Prewitt2D();

};

#endif // GRADIENTKERNEL_H
