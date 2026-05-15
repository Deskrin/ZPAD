#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "FaceDetector.hpp"
#include <chrono>
#include <iostream>

int brightness = 50;
int saturation = 50;

void onTrackbar(int, void*) {}

int main() {
    int camIndex = 0;
    std::cout << "Vvedit indeks kamery: ";
    std::cin >> camIndex;

    CameraProvider camera(camIndex);

    if (!camera.isOpened()) {
        return -1;
    }

    Display display("Lab 7");
    KeyProcessor kp;
    FrameProcessor fp;
    FaceDetector fd;

    cv::createTrackbar("Brightness", "Lab 7", &brightness, 100, onTrackbar);
    cv::createTrackbar("Saturation", "Lab 7", &saturation, 100, onTrackbar);

    fd.start();

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        auto now = std::chrono::high_resolution_clock::now();
        double fps = 1.0 / std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        if (kp.getCurrentMode() == Mode::FACE) {
            fd.setFrame(frame);
        }

        std::vector<cv::Rect> faces = fd.getFaces();
        cv::Mat res = fp.process(frame, kp.getCurrentMode(), brightness, saturation, fps, faces);

        display.show(res);

        int key = cv::waitKey(1);
        if (key == 27) break;
        kp.processKey(key);
    }

    fd.stop();
    return 0;
}
