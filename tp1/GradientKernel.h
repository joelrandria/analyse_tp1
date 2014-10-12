#ifndef GRADIENTKERNEL_H
#define GRADIENTKERNEL_H

#include <vector>
#include <opencv2/core/core.hpp>

#include "Mask.h"

class GradientKernel
{
private:

    static GradientKernel* _prewitt2d;

    MaskVec _masks;

public:

    const MaskVec& masks() const { return _masks; }

    static const GradientKernel& Prewitt2D();

};

#endif // GRADIENTKERNEL_H
