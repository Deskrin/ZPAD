#include "FaceDetector.hpp"
#include <chrono>

FaceDetector::FaceDetector() : running(false), newFrameAvailable(false) {
    net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
}

FaceDetector::~FaceDetector() {
    stop();
}

void FaceDetector::start() {
    running = true;
    workerThread = std::thread(&FaceDetector::worker, this);
}

void FaceDetector::stop() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::setFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(mtx);
    currentFrame = frame.clone();
    newFrameAvailable = true;
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(mtx);
    return detectedFaces;
}

void FaceDetector::worker() {
    while (running) {
        cv::Mat frame;
        bool process = false;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (newFrameAvailable && !currentFrame.empty()) {
                frame = currentFrame.clone();
                newFrameAvailable = false;
                process = true;
            }
        }

        if (process) {
            cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
            net.setInput(blob);
            cv::Mat detections = net.forward();
            cv::Mat detectionMat(detections.size[2], detections.size[3], CV_32F, detections.ptr<float>());

            std::vector<cv::Rect> faces;
            for(int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);
                if(confidence > 0.5) {
                    int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                    int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                    int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                    int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                    faces.push_back(cv::Rect(x1, y1, x2 - x1, y2 - y1));
                }
            }

            {
                std::lock_guard<std::mutex> lock(mtx);
                detectedFaces = faces;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
