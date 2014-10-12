#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <string>

#include "Image.h"

class Workspace
{
private:

    Image _image;

public:

    Workspace();
    Workspace(const std::string& imagePath);

    const Image& image() const { return _image; }

public:

    void loadImage(const std::string& imagePath);
};

#endif // WORKSPACE_H
