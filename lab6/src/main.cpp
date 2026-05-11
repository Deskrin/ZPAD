#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include <chrono>

int brightness = 50;
int saturation = 50;

void onTrackbar(int, void*) {}

int main() {
    CameraProvider camera(0);
    Display display("Lab 6");
    KeyProcessor kp;
    FrameProcessor fp;

    cv::createTrackbar("Brightness", "Lab 6", &brightness, 100, onTrackbar);
    cv::createTrackbar("Saturation", "Lab 6", &saturation, 100, onTrackbar);

    if (!camera.isOpened()) return -1;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        auto now = std::chrono::high_resolution_clock::now();
        double fps = 1.0 / std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        cv::Mat res = fp.process(frame, kp.getCurrentMode(), brightness, saturation, fps);

        display.show(res);

        int key = cv::waitKey(30);
        if (key == 27) break;
        kp.processKey(key);
    }
    return 0;
}
