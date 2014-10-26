#include "GradientMapMax.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <math.h>

using namespace std;

GradientMapMax::GradientMapMax(const Image &image, const GradientKernel &kernel, const bool normaliser) : GradientMap(image,kernel)
{
    float normalizationFactor = kernel.getNormalizationFactor();
    int maxValueIndex;
    float MaxValue;

    resize();

    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            Gradient gradient = getGradientAt(row, col);

            maxValueIndex = gradient.getMaxValueDirection();
            MaxValue = abs(gradient.getValueAt(maxValueIndex));

            if(normaliser == true) MaxValue *= normalizationFactor;

            _maxGradients[row][col].val = MaxValue;
            _maxGradients[row][col].dir = maxValueIndex;
        }
    }
}

void GradientMapMax::seuillageHyest(float seuilH, float seuilBas)
{
    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            if(_maxGradients[row][col].val < seuilH)//ou egale
                _maxGradients[row][col].valS = 0;
            else _maxGradients[row][col].valS = 255;
        }
    }

    seuillageBas(seuilBas);
}

void GradientMapMax::affinage()
{
    affinageX();
    affinageY();
    affinageYX();
    affinageX_Y();
    affinageV4();
}


PixelGradientInfo& GradientMapMax::composantAt(int row, int col)
{
    return _maxGradients[row][col];
}

void GradientMapMax::Bresenham(int row1,int col1,int row2,int col2)
{
    int drow, dcol, i, e;
    int incR, incC, inc1, inc2;
    int row,col;

    drow = abs(row2 - row1);
    dcol = abs(col2 - col1);

    incR = 1;
    if(row2 < row1) incR = -1;
    incC = 1;
    if(col2 < col1) incC = -1;
    row=row1;
    col=col1;

    _maxGradients[row][col].valS = 200;

    if(drow > dcol)
    {
//        _maxGradients[row][col].valS = 200;
        e = 2*dcol - drow;
        inc1 = 2*( dcol -drow);
        inc2 = 2*dcol;
        for(i = 0; i < drow; i++)
        {
            if(e >= 0)
            {
                col += incC;
                e += inc1;
            }
            else e += inc2;
            row += incR;
            _maxGradients[row][col].valS = 200;
        }
    }
    else
    {
//        _maxGradients[row][col].valS = 200;
        e = 2*drow - dcol;
        inc1 = 2*( drow - dcol);
        inc2 = 2*drow;
        for(i = 0; i < dcol; i++)
        {
            if(e >= 0)
            {
                row += incR;
                e += inc1;
            }
            else e += inc2;
            col += incC;
            //draw_pixel(x,y, BLACK);
            _maxGradients[row][col].valS = 200;
        }
    }
}

/***********PRIVATE**************/

void GradientMapMax::affinageX()
{
    for (int row = 0; row < _rows; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 0))
            {
                if((_maxGradients[row][col].val < _maxGradients[row][col-1].val) || (_maxGradients[row][col].val < _maxGradients[row][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageY()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 0; col < _cols; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 1))
            {
                if((_maxGradients[row][col].val < _maxGradients[row-1][col].val)||(_maxGradients[row][col].val < _maxGradients[row+1][col].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageYX()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 2))
            {
                if((_maxGradients[row][col].val < _maxGradients[row+1][col-1].val)||(_maxGradients[row][col].val < _maxGradients[row-1][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageX_Y()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 3))
            {
                if((_maxGradients[row][col].val < _maxGradients[row-1][col-1].val)||(_maxGradients[row][col].val < _maxGradients[row+1][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageV4()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if(_maxGradients[row][col].valS == 255)
            {
                if((_maxGradients[row-1][col].valS == 255)&&(_maxGradients[row][col+1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row-1][col].valS == 255)&&(_maxGradients[row][col-1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row+1][col].valS == 255)&&(_maxGradients[row][col-1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row+1][col].valS == 255)&&(_maxGradients[row][col+1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}

            }
        }
    }
}


void GradientMapMax::resize()
{
    _maxGradients.resize(_rows);
    for(int i = 0; i < _rows; i++)
        _maxGradients[i].resize(_cols);
}

bool GradientMapMax::testeVoisin(int x, int y)
{
    for (int i= x-1; i <= x+1; i++)
    {
        for(int j=y-1; j <= y+1; j++)
        {
            if(_maxGradients[i][j].valS == 255) return true;
        }
    }
    return false;
}

void GradientMapMax::seuillageBas(float seuilB)
{
    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            if(_maxGradients[row][col].val > seuilB)//ou egale
                if(testeVoisin(row, col) == true)
                    _maxGradients[row][col].valS = 255;
        }
    }
}

void GradientMapMax::sauveGradient(string chemainDeSauve ) const
{
    ofstream newMap(chemainDeSauve.c_str());

    if (newMap)
    {
        for(int i = 60; i < 110; i++)
        {
            for(int j = 60; j < 110; j++)
            {
                newMap << _maxGradients[i][j].valS << " ";
            }
            newMap << endl;
        }


    }else printf("ERREUR: Impossible d'ouvrir le fichier3 \n");

    newMap.close();
}
