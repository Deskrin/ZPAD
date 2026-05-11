#include "FrameProcessor.hpp"

cv::Mat FrameProcessor::process(const cv::Mat& input, Mode mode, int brightness, int saturation, double fps) {
    cv::Mat out;

    switch (mode) {
        case Mode::NORMAL: 
            out = input.clone(); 
            break;
        case Mode::INVERT: 
            cv::bitwise_not(input, out); 
            break;
        case Mode::BLUR: 
            cv::GaussianBlur(input, out, cv::Size(15, 15), 0); 
            break;
        case Mode::CANNY:
            cv::cvtColor(input, out, cv::COLOR_BGR2GRAY);
            cv::Canny(out, out, 50, 150);
            cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
            break;
        case Mode::SEPIA: {
            cv::Mat kernel = (cv::Mat_<float>(3, 3) << 
                0.272, 0.534, 0.131,
                0.349, 0.686, 0.168,
                0.393, 0.769, 0.189);
            cv::transform(input, out, kernel);
            break;
        }
        case Mode::SOBEL: {
            cv::Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y;
            cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, grad_x, CV_16S, 1, 0);
            cv::Sobel(gray, grad_y, CV_16S, 0, 1);
            cv::convertScaleAbs(grad_x, abs_grad_x);
            cv::convertScaleAbs(grad_y, abs_grad_y);
            cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);
            cv::cvtColor(out, out, cv::COLOR_GRAY2BGR);
            break;
        }
    }

    cv::cvtColor(out, out, cv::COLOR_BGR2HSV);
    for (int i = 0; i < out.rows; i++) {
        for (int j = 0; j < out.cols; j++) {
            cv::Vec3b &pixel = out.at<cv::Vec3b>(i, j);
            float s = pixel[1] * (saturation / 50.0);
            pixel[1] = (s > 255) ? 255 : (uchar)s;
        }
    }
    cv::cvtColor(out, out, cv::COLOR_HSV2BGR);

    out.convertTo(out, -1, 1, brightness - 50);

    cv::putText(out, "FPS: " + std::to_string((int)fps), cv::Point(10, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
    
    return out;
}
