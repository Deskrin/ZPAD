#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>

class CameraProvider {
public:
    CameraProvider(int deviceID = 0);
    ~CameraProvider();
    cv::Mat getFrame();
    bool isOpened() const;

private:
    cv::VideoCapture cap;
};

#endif
