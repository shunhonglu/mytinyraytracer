#ifndef TEXTURE_H
#define TEXTURE_H

#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <iostream>
#include <memory>

#include "Perlin.h"
#include "mymath.h"

class Texture {
public:
    virtual Color3d value(double u, double v, Vector3d point) const = 0;
};

class Solid_Texture : public Texture {
public:
    Solid_Texture() = default;
    Solid_Texture(Color3d color_value) : _color_value(color_value) {}
    Solid_Texture(double r, double g, double b) : _color_value(Color3d{r, g, b}) {}

    virtual Color3d value(double u, double v, Vector3d point) const override { return _color_value; }

private:
    Color3d _color_value;
};

class Check_Texture : public Texture {
public:
    Check_Texture() = default;
    Check_Texture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) : even(_even), odd(_odd) {}

    Check_Texture(Color3d c1, Color3d c2)
        : even(std::make_shared<Solid_Texture>(c1)), odd(std::make_shared<Solid_Texture>(c2)) {}

    virtual Color3d value(double u, double v, Vector3d p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class Perlin_Texture : public Texture {
public:
    Perlin_Texture() = default;
    Perlin_Texture(double scale) : _scale(scale) {}
    virtual Color3d value(double u, double v, Vector3d point) const override {
        return Color3d{1.0, 1.0, 1.0} * 0.5 * (1 + std::sin(_scale * point.z() + 10 * p.turb(point)));
    }

private:
    Perlin p;
    double _scale;
};

class Image_Texture : public Texture {
public:
    const static int desired_channels = 3;

    Image_Texture() : _data(nullptr), width(0), height(0) {}
    Image_Texture(const Image_Texture&) = delete;
    Image_Texture(Image_Texture&&) = delete;

    Image_Texture(const char* filename) {
        int channels_in_file;
        _data = stbi_load(filename, &width, &height, &channels_in_file, desired_channels);

        if (!_data) {
            spdlog::error("Can't load image texture {}!", filename);
        }
    }

    ~Image_Texture() { delete _data; }

    virtual Color3d value(double u, double v, Vector3d point) const override {
        if (!_data) {
            return {0.0, 1.0, 1.0};
        }

        /*
         * *stbi_load()* loads image from top-left,
         * but we read texture from bottom-left
         */

        u = std::clamp(u, 0.0, 1.0);
        v = 1 - std::clamp(v, 0.0, 1.0);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        auto scale = 1.0 / 255.0;
        auto pixel = _data + (j * width + i) * desired_channels;

        return {scale * pixel[0], scale * pixel[1], scale * pixel[2]};
    }

private:
    unsigned char* _data;
    int width, height;
};

#endif  // TEXTURE_H
