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

    void fermeture();

    void resetConnectedComponentsInfo();

    void sauveGradient(std::string chemainDeSauve ) const;

private:

    void resize();

    void seuillageBas(float seuilB);
    bool testeVoisin(int x, int y);

    void affinageX();
    void affinageY();
    void affinageYX();
    void affinageX_Y();
    void affinageV4();

    Pixel neigbordPos(Pixel pix);
    Pixel recherchExtr(Pixel origine, int direction, Pixel preced);
    Pixel propagation(Pixel precedent, Pixel origine);
    double prodScal(Pixel si, Pixel ori, Pixel dir);
};

#endif // GRADIENTMAPMAX_H
