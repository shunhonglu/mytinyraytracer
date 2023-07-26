#ifndef MICOROFACET_H
#define MICOROFACET_H

#include "Material.h"

class Micorofacet : public Material {
public:
    Micorofacet(double index_of_refraction, Color3d kd, Color3d ks, Color3d albedo, Color3d roughness)
        : _index_of_refraction(index_of_refraction),
          _kd(std::make_shared<Solid_Texture>(kd)),
          _ks(std::make_shared<Solid_Texture>(ks)),
          _albedo(std::make_shared<Solid_Texture>(albedo)),
          _roughness(std::make_shared<Solid_Texture>(roughness)) {}

    Micorofacet(double index_of_refraction, std::shared_ptr<Texture> kd, std::shared_ptr<Texture> ks,
                std::shared_ptr<Texture> albedo, std::shared_ptr<Texture> roughness)
        : _index_of_refraction(index_of_refraction), _kd(kd), _ks(ks), _albedo(albedo), _roughness(roughness) {}

    virtual bool scatter(const Ray& ray_in, const Hit_record& rec, Color3d& attenuation, Ray& scattered) const override {
        return false;
    }

    virtual Vector3d eval(Vector3d wi_dir, Vector3d wo_dir, const Hit_record& rec) const override;

private:
    double _index_of_refraction;
    std::shared_ptr<Texture> _kd;
    std::shared_ptr<Texture> _ks;
    std::shared_ptr<Texture> _albedo;
    std::shared_ptr<Texture> _roughness;
};

#endif  // MICOROFACET_H
