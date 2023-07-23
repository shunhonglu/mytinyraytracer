#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "mymath.h"

class Camera {
public:
    Camera() = default;
    Camera(Vector3d lookfrom, Vector3d lookat, Vector3d vup, double vfov, double aspect_ratio, double aperture,
           double focus_dist, double time0, double time1) {
        w = (lookfrom - lookat).normalized();
        u = (vup.cross(w)).normalized();
        v = (w.cross(u)).normalized();

        auto vfov_radians = degrees_to_radians(vfov);
        auto viewport_height = 2 * std::tan(vfov_radians / 2);
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2.0;
        t0 = time0;
        t1 = time1;
    }

    Ray get_ray(double u, double v) {
        Vector3d offset = lens_radius * random_in_unit_disk();
        return Ray{origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset,
                   random_double(t0, t1)};
    }

private:
    Vector3d origin;
    Vector3d horizontal;
    Vector3d vertical;
    Vector3d lower_left_corner;
    Vector3d w, u, v;
    double lens_radius;
    double t0, t1;
};

#endif  // CAMERA_H
