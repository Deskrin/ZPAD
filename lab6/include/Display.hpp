#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

class Display {
public:
    Display(const std::string& windowName = "Lab 6");
    void show(const cv::Mat& frame);

private:
    std::string name;
};

#endif
