#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int deviceID) { 
    cap.open(deviceID); 
    
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        
        std::cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
        std::cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
        std::cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << std::endl;
    }
}

CameraProvider::~CameraProvider() { 
    if (cap.isOpened()) cap.release(); 
}

cv::Mat CameraProvider::getFrame() { 
    cv::Mat frame; 
    cap >> frame; 
    return frame; 
}

bool CameraProvider::isOpened() const { 
    return cap.isOpened(); 
}
