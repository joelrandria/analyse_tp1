#ifndef GRADIENTMAPMAX_H
#define GRADIENTMAPMAX_H

#include "GradientMap.h"
#include "PixelGradientInfo.h"

class GradientMapMax : public GradientMap
{
private:

    std::vector<std::vector<PixelGradientInfo> > _maxGradients;

public:

    GradientMapMax(const Image& image, const GradientKernel& kernel, const bool normaliser=false);

    PixelGradientInfo& composantAt(int row, int col);

    void seuillageHyest (float seuilH, float seuilBas);

    void affinage ();

    void sauveGradient(std::string chemainDeSauve ) const;

private:

    void resize();

    void seuillageBas(float seuilB);
    bool testeVoisin(int x, int y);

    void affinageX();
    void affinageY();
    void affinageYX();
    void affinageX_Y();

};

#endif // GRADIENTMAPMAX_H
