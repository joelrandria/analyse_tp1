#ifndef PIXELGRADIENTINFO_H
#define PIXELGRADIENTINFO_H

class PixelGradientInfo
{
public:

    // Attributs spécifiques au gradient
    int dir;
    float val;
    float valS;

    // Attributs spécifiques à la connexité
    bool end = false;
    int connectedComponentId = -1;

    bool visite = false;

public:

    PixelGradientInfo()
    {
        resetConnectedComponentInfo();
    }

    bool isAccepted() const { return (valS > 0); }
    bool isInConnectedComponent() const { return (connectedComponentId >= 0); }

    void resetConnectedComponentInfo()
    {
        end = false;
        connectedComponentId = -1;
    }
};

#endif // PIXELGRADIENTINFO_H
