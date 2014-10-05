#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#include <opencv2/core/core.hpp>

class Image
{
private:

    cv::Mat _matrix;
    cv::Mat _grayscaleMatrix;

public:

    Image();

    const cv::Mat& matrix() const { return _matrix; }
    const cv::Mat& grayscaleMatrix() const { return _grayscaleMatrix; }

    void load(const std::string& imagePath);
};

#endif // IMAGE_H
