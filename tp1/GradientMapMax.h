#ifndef GRADIENTMAPMAX_H
#define GRADIENTMAPMAX_H

#include "GradientMap.h"
#include "PixelGradientInfo.h"

typedef struct Pixel Pixel;
struct Pixel
{
    int r;
    int c;
};

class GradientMapMax : public GradientMap
{
private:

    std::vector<std::vector<PixelGradientInfo> > _maxGradients;

public:

    GradientMapMax(const Image& image, const GradientKernel& kernel, const bool normaliser=false);

    PixelGradientInfo& composantAt(int row, int col);

    void seuillageHyest (float seuilH, float seuilBas);

    void affinage ();

    void Bresenham(int row1, int col1, int row2, int col2);

//    void fermetureSimple();

    void sauveGradient(std::string chemainDeSauve ) const;

private:

    void resize();

    void seuillageBas(float seuilB);
    bool testeVoisin(int x, int y);
//    bool connexe(int row, int col);


//    bool extremites(int row, int col);

    void affinageX();
    void affinageY();
    void affinageYX();
    void affinageX_Y();
    void affinageV4();

};

#endif // GRADIENTMAPMAX_H
