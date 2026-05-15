#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();
    void start();
    void stop();
    void setFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();

private:
    void worker();

    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex mtx;
    std::atomic<bool> running;
    std::atomic<bool> newFrameAvailable;

    cv::Mat currentFrame;
    std::vector<cv::Rect> detectedFaces;
};

#endif
