#include "GradientKernel.h"

#include <iostream>

/************DEFINITION DES MASKS*************************/

GradientKernel* GradientKernel::_prewitt2d = 0;
GradientKernel* GradientKernel::_prewitt4d = 0;//4D

Mask prewittDir0Mask = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
Mask prewittDir1Mask = {{0, 1, 1}, {-1, 0, 1}, {-1, -1, 0}};
Mask prewittDir2Mask = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};
Mask prewittDir3Mask = {{1, 1, 0}, {1, 0, -1}, {0, -1, -1}};

GradientKernel* GradientKernel::_sobel2d = 0;
GradientKernel* GradientKernel::_sobel4d = 0;//4D

Mask sobelDir0Mask = {{-1,0,1},{-2,0,2},{-1,0,1}};
Mask sobelDir1Mask = {{0,1,2},{-1,0,1},{-2,-1,0}};
Mask sobelDir2Mask = {{1,2,1},{0,0,0},{-1,-2,-1}};
Mask sobelDir3Mask = {{2,1,0},{1,0,-1},{0,-1,-2}};

GradientKernel* GradientKernel::_kirsch2d = 0;
GradientKernel* GradientKernel::_kirsch4d = 0;//4D

Mask kirschDir0Mask = {{-3,-3,5},{-3,0,5},{-3,-3,5}};
Mask kirschDir1Mask = {{-3,5,5},{-3,0,5},{-3,-3,-3}};
Mask kirschDir2Mask = {{5,5,5},{-3,0,-3},{-3,-3,-3}};
Mask kirschDir3Mask = {{5,5,-3},{5,0,-3},{-3,-3,-3}};

/******FIN DE DEFINITION DES MASKS*****************************/

/************2D***********************************************/
const GradientKernel& GradientKernel::Prewitt2D()
{
    if (_prewitt2d == 0)
    {
        _prewitt2d = new GradientKernel();
        _prewitt2d->_masks.push_back(prewittDir0Mask);//position 0 ds MaskVec
        _prewitt2d->_masks.push_back(prewittDir2Mask);//position 1 ds MaskVec

        _prewitt2d->_normalizationFactor = 1.f/3.f;
    }

    return *_prewitt2d;
}///////////prewitt 2D


const GradientKernel& GradientKernel::Sobel2D()
{
    if (_sobel2d == 0)
    {
        _sobel2d = new GradientKernel();
        _sobel2d->_masks.push_back(sobelDir0Mask);
        _sobel2d->_masks.push_back(sobelDir2Mask);

        _sobel2d->_normalizationFactor = 1.f/4.f;
    }

    return *_sobel2d;
}//////////sobel 2D

const GradientKernel& GradientKernel::Kirsch2D()
{
    if(_kirsch2d == 0)
    {
        _kirsch2d = new GradientKernel();
        _kirsch2d-> _masks.push_back(kirschDir0Mask);
        _kirsch2d->_masks.push_back(kirschDir2Mask);

        _kirsch2d->_normalizationFactor = 1.f/15.f;
    }

    return *_kirsch2d;
}/////////kirsch 2D
/**********************4D*********************************************/
const GradientKernel& GradientKernel::Prewitt4D()
{
    if (_prewitt4d == 0)
    {
        _prewitt4d = new GradientKernel();
        _prewitt4d->_masks.push_back(prewittDir0Mask);//position 0 ds MaskVec
        _prewitt4d->_masks.push_back(prewittDir2Mask);//position 1 ds MaskVec
        _prewitt4d->_masks.push_back(prewittDir1Mask);//position 2 ds MaskVec
        _prewitt4d->_masks.push_back(prewittDir3Mask);//position 3 ds MaskVec

        _prewitt4d->_normalizationFactor = 1.f/3.f;
    }

    return *_prewitt4d;
}///////////prewitt 4D


const GradientKernel& GradientKernel::Sobel4D()
{
    if (_sobel4d == 0)
    {
        _sobel4d = new GradientKernel();
        _sobel4d->_masks.push_back(sobelDir0Mask);
        _sobel4d->_masks.push_back(sobelDir2Mask);
        _sobel4d->_masks.push_back(sobelDir1Mask);
        _sobel4d->_masks.push_back(sobelDir3Mask);

        _sobel4d->_normalizationFactor = 1.f/4.f;
    }

    return *_sobel4d;
}//////////sobel 4D

const GradientKernel& GradientKernel::Kirsch4D()
{
    if(_kirsch4d == 0)
    {
        _kirsch4d = new GradientKernel();
        _kirsch4d-> _masks.push_back(kirschDir0Mask);
        _kirsch4d->_masks.push_back(kirschDir2Mask);
        _kirsch4d->_masks.push_back(kirschDir1Mask);
        _kirsch4d->_masks.push_back(kirschDir3Mask);

        _kirsch4d->_normalizationFactor = 1.f/15.f;
    }

    return *_kirsch4d;
}/////////kirsch 4D
