#include "Workspace.h"

#include <opencv2/highgui/highgui.hpp>


Workspace::Workspace()
{
}
Workspace::Workspace(const std::string& imagePath)
{
    loadImage(imagePath);
}

const cv::Mat& Workspace::originalImage() const
{
    return _originalImage;
}
const cv::Mat& Workspace::grayscaleImage() const
{
    return _grayscaleImage;
}

void Workspace::loadImage(const std::string& imagePath)
{
    _originalImage = cv::imread(imagePath);
    _grayscaleImage = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
}
