#include "GradientMapMax.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

GradientMapMax::GradientMapMax(const Image &image, const GradientKernel &kernel, const bool normaliser) : GradientMap(image,kernel)
{
    float normalizationFactor = kernel.getNormalizationFactor();
    int maxValueIndex;
    float MaxValue;

    resize();//initialisation de _maxGradientMap

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
}


const Composant GradientMapMax::getComposantAt(int row, int col) const
{
    return _maxGradients[row][col];
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
                if((_maxGradients[row][col].dir == _maxGradients[row][col-1].dir) && (_maxGradients[row][col].val > _maxGradients[row][col-1].val))
                {
                    _maxGradients[row][col-1].valS = 0;
                }
                if((_maxGradients[row][col].dir == _maxGradients[row][col+1].dir) && (_maxGradients[row][col].val > _maxGradients[row][col+1].val))
                {
                    _maxGradients[row][col+1].valS = 0;
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
                if((_maxGradients[row][col].dir == _maxGradients[row-1][col].dir) && (_maxGradients[row][col].val > _maxGradients[row-1][col].val))
                {
                    _maxGradients[row-1][col].valS = 0;
                }
                if((_maxGradients[row][col].dir == _maxGradients[row+1][col].dir) && (_maxGradients[row][col].val > _maxGradients[row+1][col].val))
                {
                    _maxGradients[row+1][col].valS = 0;
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
                if((_maxGradients[row][col].dir == _maxGradients[row+1][col-1].dir) && (_maxGradients[row][col].val > _maxGradients[row+1][col-1].val))
                {
                    _maxGradients[row+1][col-1].valS = 0;
                }
                if((_maxGradients[row][col].dir == _maxGradients[row-1][col+1].dir) && (_maxGradients[row][col].val > _maxGradients[row-1][col+1].val))
                {
                    _maxGradients[row-1][col+1].valS = 0;
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
                if((_maxGradients[row][col].dir == _maxGradients[row-1][col-1].dir) && (_maxGradients[row][col].val > _maxGradients[row-1][col-1].val))
                {
                    _maxGradients[row-1][col-1].valS = 0;
                }
                if((_maxGradients[row][col].dir == _maxGradients[row+1][col+1].dir) && (_maxGradients[row][col].val > _maxGradients[row+1][col+1].val))
                {
                    _maxGradients[row+1][col+1].valS = 0;
                }

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
//   bool res = false;
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
/*****JUST FOR TEST**********/
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
