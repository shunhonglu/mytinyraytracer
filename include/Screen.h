#ifndef SCREEN_H
#define SCREEN_H

#include "mymath.h"
#include <vector>

class Screen {
public:
    Screen(int width = 0, int height = 0) : _width(width), _height(height), _data(std::vector<unsigned char>(3 * width * height)) {}
    void write_color(int i, int j, Color3d color, int samples_per_pixel) {
        size_t index = _width * (_height - j - 1) + i;
        auto scale = 1.0 / samples_per_pixel;

        _data[3 * index] = static_cast<unsigned char>(std::clamp(std::sqrt(color.x() * scale), 0.0, 1.0) * 255.9);
        _data[3 * index + 1] = static_cast<unsigned char>(std::clamp(std::sqrt(color.y() * scale), 0.0, 1.0) * 255.9);
        _data[3 * index + 2] = static_cast<unsigned char>(std::clamp(std::sqrt(color.z() * scale), 0.0, 1.0) * 255.9);
    }

    int get_width() const { return _width;}
    int get_height() const {return _height;}
    const std::vector<unsigned char>& get_data() const { return _data; }

private:
    int _width{0};
    int _height{0};
    std::vector<unsigned char> _data;
};

#endif // SCREEN_H
