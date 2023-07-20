#include "framebuffer.h"

void FrameBuffer::set_color(int row, int column, Color3d color) {
    for(size_t i = 0; i<3;++i) {
        data[row * width + column + i] = color.x();
    }
}

void FrameBuffer::fill(Color3d color) {
    for(size_t i = 0;i< width;++i) {
        for(size_t j = 0;j < height;++j) {
            set_color(i, j, color);
        }
    }
}
