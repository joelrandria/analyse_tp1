#include "GradientKernel.h"

#include <iostream>

GradientKernel* GradientKernel::_prewitt2d = 0;

Mask prewittDir0Mask = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
Mask prewittDir1Mask = {{0, 1, 1}, {-1, 0, 1}, {-1, -1, 0}};
Mask prewittDir2Mask = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};
Mask prewittDir3Mask = {{1, 1, 0}, {1, 0, -1}, {0, -1, -1}};

const GradientKernel& GradientKernel::Prewitt2D()
{
    if (_prewitt2d == 0)
    {
        _prewitt2d = new GradientKernel();
        _prewitt2d->_masks.push_back(prewittDir0Mask);
        _prewitt2d->_masks.push_back(prewittDir2Mask);

        _prewitt2d->_normalizationFactor = 1.f/3.f;
    }

    return *_prewitt2d;
}
