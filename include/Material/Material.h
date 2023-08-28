#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hittable.h"
#include "Ray.h"
#include "Texture.h"
#include "mymath.h"

class Material {
public:
    virtual Vector3d scatter(const Ray& ray_in, const Hit_record& rec, Ray& scattered) const {
        return Vector3d{0.0, 0.0, 0.0};
    }
    /*
     * This function used for calculating BRDF.
     * wi_dir always point out, wo_dir always point in.
     * and they may not unit vector. You should normalize it by yourself.
     */
    virtual Vector3d eval(Vector3d wi_dir, Vector3d wo_dir, const Hit_record& rec) const {
        return Vector3d{0.0, 0.0, 0.0};
    }
    // These two functions only need to be overloaded for illuminated Matreial such as Diffuse_light.
    virtual Color3d emitted(double u, double v, Vector3d point) const { return Color3d{0.0, 0.0, 0.0}; }
    virtual bool has_emit() const { return false; }
    virtual bool use_ray_tracing() const { return false; }
};

class Lambertian : public Material {
public:
    Lambertian(Color3d color) : albedo(std::make_shared<Solid_Texture>(color)) {}
    Lambertian(std::shared_ptr<Texture> texture) : albedo(texture) {}

    virtual Vector3d eval(Vector3d wi_dir, Vector3d wo_dir, const Hit_record& rec) const override {
        Vector3d unit_wi_dir = wi_dir.normalized();
        double cos_theta = unit_wi_dir.dot(rec.normal);

        if (cos_theta > 0.0) {
            return albedo->value(rec.u, rec.v, rec.p) / pi;
        } else {
            return Vector3d{0.0, 0.0, 0.0};
        }
    }

private:
    std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
    Metal(Color3d color, double f = 0.0) : albedo(std::make_shared<Solid_Texture>(color)), fuzzy(f) {}
    Metal(std::shared_ptr<Texture> texture, double f = 0.0) : albedo(texture), fuzzy(f) {}

    virtual Vector3d scatter(const Ray& ray_in, const Hit_record& rec, Ray& scattered) const override {
        Vector3d scattered_direction = reflect(ray_in.direction(), rec.normal);

        scattered = Ray{rec.p, scattered_direction + fuzzy * random_unit_vector(), ray_in.time()};
        // Attention: because *fuzzy* is too big to we scatter below the surface,
        // so we need to judge if *reflection* is the same side as *normal*
        if (scattered.direction().dot(rec.normal) > 0) {
            return albedo->value(rec.u, rec.v, rec.p);
        } else {
            return Vector3d{0.0, 0.0, 0.0};
        }
    }

    virtual bool use_ray_tracing() const override { return true; }

private:
    std::shared_ptr<Texture> albedo;
    double fuzzy;
};

// class Dielectric : public Material {
// public:
//     Dielectric(double ir) : index_of_refraction(ir) {}

//    virtual bool scatter(const Ray& ray_in, const Hit_record& rec, Color3d& attenuation,
//                         Ray& scattered) const override {
//        Vector3d unit_ray_in_dir = ray_in.direction().normalized();
//        double eta_over_etap = (rec.front_face) ? (1 / index_of_refraction) : (index_of_refraction);

//        // total internal refection
//        double cos_theta = std::min(-unit_ray_in_dir.dot(rec.normal), 1.0);
//        double sin_theta = std::sqrt(1 - std::pow(cos_theta, 2));
//        bool cannot_refract = eta_over_etap * sin_theta > 1.0;

//        Vector3d scattered_direction;
//        if (cannot_refract || reflectance(eta_over_etap, cos_theta) > random_double()) {
//            scattered_direction = reflect(unit_ray_in_dir, rec.normal);
//        } else {
//            scattered_direction = refract(unit_ray_in_dir, rec.normal, eta_over_etap);
//        }

//        scattered = Ray{rec.p, scattered_direction, ray_in.time()};
//        attenuation = Color3d{1.0, 1.0, 1.0};

//        return true;
//    }

// private:
//     static double reflectance(double eta_over_etap, double cos_theta) {
//         double r0 = (eta_over_etap - 1) / (eta_over_etap + 1);
//         r0 = r0 * r0;
//         return r0 + (1 - r0) * std::pow((1 - cos_theta), 5);
//     }

//    double index_of_refraction;
//};

#endif  // MATERIAL_H
