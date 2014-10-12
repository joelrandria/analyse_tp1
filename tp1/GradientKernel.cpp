#include "GradientKernel.h"

GradientKernel* GradientKernel::_prewitt2d = 0;

int prewittDir0Mask[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
int prewittDir1Mask[3][3] = {{0, 1, 1}, {-1, 0, 1}, {-1, -1, 0}};
int prewittDir2Mask[3][3] = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};
int prewittDir3Mask[3][3] = {{1, 1, 0}, {1, 0, -1}, {0, -1, -1}};

const GradientKernel& GradientKernel::Prewitt2D()
{
    if (_prewitt2d == 0)
    {
        _prewitt2d = new GradientKernel();
        _prewitt2d->_masks.push_back((int**)prewittDir0Mask);
        _prewitt2d->_masks.push_back((int**)prewittDir2Mask);
    }

    return *_prewitt2d;
}
