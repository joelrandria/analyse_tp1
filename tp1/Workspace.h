#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <string>

#include <opencv2/core/core.hpp>

class Workspace
{
private:

    cv::Mat _originalImage;
    cv::Mat _grayscaleImage;

public:

    Workspace();
    Workspace(const std::string& imagePath);

    const cv::Mat& originalImage() const;
    const cv::Mat& grayscaleImage() const;

public:

    void loadImage(const std::string& imagePath);
};

#endif // WORKSPACE_H
