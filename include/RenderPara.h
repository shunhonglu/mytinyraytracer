#ifndef RENDERPARA_H
#define RENDERPARA_H

#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <unordered_map>

#include "Texture.h"
#include "mymath.h"
#include "utils.h"

using json = nlohmann::json;

struct RenderPara {
    enum Texture_Type { t_Solid_Texture, t_Image_Texture };
    enum Material_Type { m_Metal, m_Lambertian, m_Diffuse_light, m_Micorofacet };

    static std::unordered_map<std::string, Texture_Type> t;
    static std::unordered_map<std::string, Material_Type> m;

    RenderPara(const std::string& path) {
        std::ifstream json_file(path);

        json json_data = json::parse(json_file);

        // Render Parameters
        samples_per_pixel = json_data["render_parameters"]["samples_per_pixel"];
        spdlog::info("samples_per_pixel: {}", samples_per_pixel);

        raytraer_depth = json_data["render_parameters"]["raytraer_depth"];
        spdlog::info("raytraer_depth: {}", raytraer_depth);

        std::vector<double> _background = json_data["render_parameters"]["background"];
        background << _background[0], _background[1], _background[2];
        spdlog::info("background: [ {} ]", Vec3d_to_str(background));

        // Screen
        build_screen(json_data["screen"]);

        // Camera
        build_camera(json_data["camera"]);

        // Scene
        build_world(json_data["scene"]);
    }

    std::shared_ptr<Texture> build_texture(const json& texture) const;
    std::shared_ptr<Material> build_material(const json& material) const;
    void build_obj(const std::string& obj_path, std::shared_ptr<Material> mat_ptr);
    void build_screen(const json& screen);
    void build_camera(const json& camera);
    void build_world(const json& scene);

    // Render Parameters
    int samples_per_pixel{1};
    int raytraer_depth{4};
    Color3d background{0.0, 0.0, 0.0};

    // Screen
    int width{0};
    int height{0};
    double aspect_ratio{0.0};
    Screen canvas;

    // Camera
    Vector3d lookfrom{0.0, 0.0, 1.0};
    Vector3d lookat{0.0, 0.0, 0.0};
    Vector3d vup{0.0, 1.0, 0.0};
    double vfov{0.0};
    double aperture{0.0};
    double focus_dist{1.0};
    Camera cam;

    // Scene
    Hittable_list world;
};

#endif  // RENDERPARA_H
