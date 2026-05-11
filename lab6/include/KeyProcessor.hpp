#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

enum class Mode { 
    NORMAL, 
    INVERT, 
    BLUR, 
    CANNY, 
    SEPIA, 
    SOBEL 
};

class KeyProcessor {
public:
    KeyProcessor();
    void processKey(int key);
    Mode getCurrentMode() const;
private:
    Mode currentMode;
};

#endif
