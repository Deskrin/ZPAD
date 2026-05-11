#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int deviceID) {
    cap.open(deviceID);
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}
