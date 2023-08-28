#ifndef LIGHT_H
#define LIGHT_H

#include "Material.h"

class Diffuse_light : public Material {
public:
    Diffuse_light() = default;
    Diffuse_light(std::shared_ptr<Texture> emit_texture) : _emit_texture(emit_texture) {}
    Diffuse_light(Color3d emit_color) : _emit_texture(std::make_shared<Solid_Texture>(emit_color)) {}

    virtual Color3d emitted(double u, double v, Vector3d point) const override {
        return _emit_texture->value(u, v, point);
    }
    virtual bool has_emit() const override { return true; }

private:
    std::shared_ptr<Texture> _emit_texture;
};

#endif  // LIGHT_H
