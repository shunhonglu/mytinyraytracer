#include "Render.h"

#include "Material.h"
#include "utils.h"

Color3d Render::ray_color(const Ray& r, Color3d background, const Hittable& world, int depth) {
    switch (1) {
        case 0: {
            if (depth == 0) {
                return Color3d{0.0, 0.0, 0.0};
            }

            Hit_record rec;
            if (world.hit(r, 0.001, infinity, rec)) {
                Ray scattered;
                Color3d attenuation;
                if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                    return attenuation.cwiseProduct(ray_color(scattered, background, world, depth - 1));
                }
                return Color3d{0.0, 0.0, 0.0};
            }
            Vector3d unit_direction = r.direction().normalized();
            auto t = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - t) * Color3d(1.0, 1.0, 1.0) + t * Color3d(0.5, 0.7, 1.0);
        } break;
        default: {
            if (depth == 0) {
                return Color3d{0.0, 0.0, 0.0};
            }

            Hit_record rec;
            if (!world.hit(r, 0.001, infinity, rec)) {
                return background;
            }

            Ray scattered;
            Color3d attenuation;
            Color3d emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
            if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return emitted;
            }

            return emitted + attenuation.cwiseProduct(ray_color(scattered, background, world, depth - 1));
        } break;
    }
}

void Render::render(int samples_per_pixel, int depth, Color3d background) {
    int width = _canvas.get_width();
    int height = _canvas.get_height();

    render_begin_time = QTime::currentTime();
    for (int j = height - 1; j >= 0; --j) {
        remaining_lines = j;
        for (int i = 0; i < width; ++i) {
            Color3d color_per_pixel{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (static_cast<double>(i) + random_double()) / (width - 1);
                auto v = (static_cast<double>(j) + random_double()) / (height - 1);
                auto ray = _cam.get_ray(u, v);
                color_per_pixel += ray_color(ray, background, bvh, depth);
            }
            _canvas.write_color(i, j, color_per_pixel, samples_per_pixel);
        }
    }

    render_completion = true;
}
