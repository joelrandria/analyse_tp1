#include "Image.h"

#include <opencv2/highgui/highgui.hpp>

void Image::load(const std::string &imagePath)
{
    _matrix = cv::imread(imagePath);
    _grayscaleMatrix = cv::imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
}
