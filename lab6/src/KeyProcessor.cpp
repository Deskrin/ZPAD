#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(Mode::NORMAL) {}

void KeyProcessor::processKey(int key) {
    switch (key) {
        case '1': currentMode = Mode::NORMAL; break;
        case '2': currentMode = Mode::INVERT; break;
        case '3': currentMode = Mode::BLUR; break;
        case '4': currentMode = Mode::CANNY; break;
        case '5': currentMode = Mode::SEPIA; break;
        case '6': currentMode = Mode::SOBEL; break;
    }
}

Mode KeyProcessor::getCurrentMode() const { return currentMode; }
