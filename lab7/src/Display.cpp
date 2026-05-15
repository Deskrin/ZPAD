#include "Display.hpp"

Display::Display(const std::string& windowName) : name(windowName) {
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow(name, frame);
    }
}
