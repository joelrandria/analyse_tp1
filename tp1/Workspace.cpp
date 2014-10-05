#include "Workspace.h"

Workspace::Workspace()
{
}
Workspace::Workspace(const std::string& imagePath)
{
    loadImage(imagePath);
}

void Workspace::loadImage(const std::string& imagePath)
{
    _image.load(imagePath);
}
