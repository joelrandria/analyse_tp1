#ifndef PIXELGRADIENTINFO_H
#define PIXELGRADIENTINFO_H

class PixelGradientInfo
{
public:

    int dir;
    float val;
    float valS;
    bool end = false;
    int connectedComponentId = -1;

public:

    bool isAccepted() const { return (valS > 0); }
    bool isInConnectedComponent() const { return (connectedComponentId >= 0); }
};

#endif // PIXELGRADIENTINFO_H
