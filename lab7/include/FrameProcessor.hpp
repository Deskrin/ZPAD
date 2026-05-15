#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"
#include <vector>

class FrameProcessor {
public:
    cv::Mat process(const cv::Mat& input, Mode mode, int brightness, int saturation, double fps, const std::vector<cv::Rect>& faces);
};

#endif
