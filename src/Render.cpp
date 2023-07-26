#include "Render.h"

#include "Material.h"
#include "PDF.h"
#include "Ray.h"
#include "spdlog/spdlog.h"
#include "utils.h"

void Render::build_lights() {
    auto objects = _world.get_objects();
    for (size_t i = 0; i < objects.size(); ++i) {
        auto object = objects[i];

        std::shared_ptr<Material> mat_temp;
        if (!object->get_mat(mat_temp)) {
            spdlog::error("Hittable_list contains *Hittable_list* or *BVH_Node*, it's invalid!");
            exit(1);
        }

        if (mat_temp->has_emit()) {
            _lights.add(object);
        }
    }

    spdlog::info("Lights in Render have been built. There are {} lights", _lights.get_objects().size());
}

Color3d Render::ray_color(const Ray& r, Color3d background, const Hittable& world, int depth) {
    if (depth == 0) {
        return Color3d{0.0, 0.0, 0.0};
    }

    Hit_record rec;
    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }

    if (rec.mat_ptr->has_emit()) {
        return rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
    }

    /*
     * DIRECT LIGHT
     */

    Color3d direct{0.0, 0.0, 0.0};  // it must be initialized.

    if (!_lights.get_objects().empty()) {
        // first randomly choose a light.
        int light_index = random_int(0, _lights.get_objects().size() - 1);
        std::shared_ptr<Hittable> light = _lights.get_objects()[light_index];

        // second sample the light.
        Hit_record light_rec;
        double light_pdf;
        if (!light->sample(light_rec, light_pdf)) {
            // TODO: How to sample a volume?
            spdlog::error("This is not a sampleable object!. It's maybe a volume.");
            exit(1);
        }

        // third judge if light is blocked and calculate the direct light.
        Vector3d unit_wi_dir = (light_rec.p - rec.p).normalized();
        double distance = (light_rec.p - rec.p).norm();
        Hit_record blocked_rec;
        if (!world.hit(Ray{rec.p, unit_wi_dir, 0.0}, 0.001, infinity, blocked_rec)) {
            spdlog::error("Ray to the light sampling point hit nothing!");
            exit(1);
        }

        double blocked_distance = (blocked_rec.p - rec.p).norm();
        if (std::abs(distance - blocked_distance) <= 0.01) {
            Color3d light_emitted = light_rec.mat_ptr->emitted(light_rec.u, light_rec.v, light_rec.p);
            double direct_cos_theta = rec.normal.dot(unit_wi_dir);
            double direct_cos_theta_prime = unit_wi_dir.dot(-light_rec.normal);
            direct = light_emitted.cwiseProduct(rec.mat_ptr->eval(unit_wi_dir, r.direction(), rec)) * direct_cos_theta *
                     direct_cos_theta_prime / std::pow(distance, 2) / light_pdf;
        }
    }

    /*
     * INDIRECT LIGHT
     */

    Color3d indirect{0.0, 0.0, 0.0};  // it must be initialized.

    if (random_double() < RussianRoulette) {
        double scale = 1 / RussianRoulette;
        Cosine_PDF cosine_pdf(rec.normal);
        Ray scattered{rec.p, cosine_pdf.generate(), r.time()};
        Hit_record is_light;
        // if indirect light hits nothing, set indirect light as background.
        if (!world.hit(scattered, 0.001, infinity, is_light)) {
            indirect = background;
            // if indirect light hits object emitting, set it as zero.
        } else if (world.hit(scattered, 0.001, infinity, is_light) && is_light.mat_ptr->has_emit()) {
            indirect = {0.0, 0.0, 0.0};
        } else {
            double pdf = cosine_pdf.value(scattered.direction());
            double cos_theta = rec.normal.dot(scattered.direction().normalized());
            indirect = ray_color(scattered, background, world, depth)
                           .cwiseProduct(rec.mat_ptr->eval(scattered.direction(), r.direction(), rec)) *
                       cos_theta / pdf * scale;
        }
    }

    return direct + indirect;
}

void Render::render(int samples_per_pixel, int depth, Color3d background) {
    int width = _canvas.get_width();
    int height = _canvas.get_height();

    render_begin_time = QTime::currentTime();
    for (int j = height - 1; j >= 0; --j) {
        remaining_lines = j;
        for (int i = 0; i < width; ++i) {
            // used for debug.
            // std::cout << i << '\t' << j << '\n';
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
