#ifndef GRADIENTMAPMAX_H
#define GRADIENTMAPMAX_H
#include "GradientMap.h"


typedef struct{
    float valS;
    float val; //j'ai utilise un float pour garder la diffirence apret normalisation
    int dir;

}Composant;

class GradientMapMax : public GradientMap
{
private:

    std::vector<std::vector<Composant> > _maxGradients;//int le abs(composant) max du gradient

public:
    GradientMapMax(const Image& image, const GradientKernel& kernel, const bool normaliser=false);

    const Composant getComposantAt(int row, int col) const;

    void seuillageHyest (float seuilH, float seuilBas); //float pr quand puis utilise par ex: 100.56 dans une interval 0-255

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
