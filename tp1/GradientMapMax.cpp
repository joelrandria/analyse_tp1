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

void GradientMapMax::seuillageFixe(float seuil)
{
    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
          if(_maxGradients[row][col].val < seuil)//ou egale
              _maxGradients[row][col].valS = 0;
          else _maxGradients[row][col].valS = 255;
        }
    }


}

void GradientMapMax::seuillageLocale()//ToDo : taille de masque argument + ds Bernsen+sauvola
{
    for (int i= 12; i < _rows-12; i++)
    {
        for(int j=12; j < _cols-12; j++)
        {
            float seuilDeFenetre = computeSeuilBernsen(i,j);
            //float seuilDeFenetre = computeSeuilSauvola(i,j);
            if(_maxGradients[i][j].val < seuilDeFenetre)
                _maxGradients[i][j].valS = 0;
            else _maxGradients[i][j].valS = 255;
        }
    }
}

const Composant GradientMapMax::getComposantAt(int row, int col) const
{
    return _maxGradients[row][col];
}


void GradientMapMax::resize()
{
    _maxGradients.resize(_rows);
    for(int i = 0; i < _rows; i++)
        _maxGradients[i].resize(_cols);
}


float GradientMapMax::computeSeuilBernsen(int x, int y) //Methode Bernsen 1986
{
    float max = 0.;
    float min = 0.;

    for(int i = x-12; i <= x+12; i++)
    {
        for(int j= y-12; j <= y+12; j++)
        {
            if(max < _maxGradients[i][j].val) max = _maxGradients[i][j].val;
            if(min > _maxGradients[i][j].val) min = _maxGradients[i][j].val;
        }
    }

    float seuil = (max + min)/2;

    return seuil;
}

float GradientMapMax::computeSeuilSauvola(int x, int y)
{
    float k=0.2;
    int R=128;

    float moy = moyFenetre(x,y);
    float ecarType = ecartType(x,y,moy);

    return moy * (1 + k * ((ecarType / R)-1));
}

float GradientMapMax::moyFenetre(int x, int y)
{
    float somme=0;
    int cpt=0;
    for(int i= x-12; i <= x+12; i++)
    {
        for(int j= y-12; j <= y+12; j++)
        {
            somme += _maxGradients[i][j].val;
            cpt++;
        }
    }
    return somme/cpt;
}

float GradientMapMax::ecartType(int x, int y, float moy)
{
    float somme=0;
    int cpt=0;
    for(int i= x-12; i <= x+12; i++)
    {
        for(int j= y-12; j <= y+12; j++)
        {
            somme += pow((_maxGradients[i][j].val - moy), 2);
            cpt++;
        }
    }

    return sqrt(somme /cpt);
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
