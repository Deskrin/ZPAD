#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    cv::Mat process(const cv::Mat& input, Mode mode, int brightness, int saturation, double fps);
};

#endif
