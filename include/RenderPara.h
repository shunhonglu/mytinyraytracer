#ifndef RENDERPARA_H
#define RENDERPARA_H

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <unordered_map>

#include "mymath.h"
#include "spdlog/spdlog.h"
#include "utils.h"

using json = nlohmann::json;

struct RenderPara {
    enum Material_Type{
        m_Lambertian, m_Diffuse_light
    };

    static std::unordered_map<std::string, Material_Type> m;

    RenderPara(const std::string& path) {
        std::ifstream json_file(path);

        json json_data = json::parse(json_file);

        // Camera
        std::vector<double> _lookfrom = json_data["camera"]["lookfrom"];
        lookfrom << _lookfrom[0], _lookfrom[1], _lookfrom[2];
        spdlog::info("lookfrom: [ {} ]", Vec3d_to_str(lookfrom));

        std::vector<double> _lookat = json_data["camera"]["lookat"];
        lookat << _lookat[0], _lookat[1], _lookat[2];
        spdlog::info("lookat: [ {} ]", Vec3d_to_str(lookat));

        std::vector<double> _vup = json_data["camera"]["vup"];
        vup << _vup[0], _vup[1], _vup[2];
        spdlog::info("vup: [ {} ]", Vec3d_to_str(vup));

        vfov = json_data["camera"]["vfov"];
        spdlog::info("vfov: {}", vfov);

        aperture = json_data["camera"]["aperture"];
        spdlog::info("aperture: {}", aperture);

        focus_dist = json_data["camera"]["focus_dist"];
        spdlog::info("focus_dist: {}", focus_dist);

        // Render Parameters
        samples_per_pixel = json_data["render_parameters"]["samples_per_pixel"];
        spdlog::info("samples_per_pixel: {}", samples_per_pixel);

        raytraer_depth = json_data["render_parameters"]["raytraer_depth"];
        spdlog::info("raytraer_depth: {}", raytraer_depth);

        std::vector<double> _background = json_data["render_parameters"]["background"];
        background << _background[0], _background[1], _background[2];
        spdlog::info("background: [ {} ]", Vec3d_to_str(background));

        // Scene
        build_world(json_data["scene"]);

//        if(json_data["scene"].find("objs") != json_data["scene"].end()) {
//            auto objs = json_data["scene"]["objs"];
//            for(const auto& obj:objs) {
//                std::string path = obj["obj_path"];
//                auto material = obj["material"];
//            }
//        }

//        scene_function = json_data["scene"]["scene_function"];

//        if(json_data["scene"].find("scene_obj_paths") != json_data["scene"].end()) {
//            scene_obj_paths = json_data["scene"]["scene_obj_paths"];
//            for(const auto& scene_obj_path : scene_obj_paths) {
//                spdlog::info("scene_obj_paths: {}", scene_obj_path);
//            }
//        }
    }

    std::shared_ptr<Material> build_material(const json& material) const;
    void build_obj(const std::string& obj_path, std::shared_ptr<Material> mat_ptr);
    void build_world(const json& scene);

    // Camera
    Vector3d lookfrom{0.0, 0.0, 1.0};
    Vector3d lookat{0.0, 0.0, 0.0};
    Vector3d vup{0.0, 1.0, 0.0};
    double vfov{0.0};
    double aperture{0.0};
    double focus_dist{1.0};

    // Render Parameters
    int samples_per_pixel{1};
    int raytraer_depth{4};
    Color3d background{0.0, 0.0, 0.0};

    // Scene
    Hittable_list world;
    std::string scene_function;
    std::vector<std::string> scene_obj_paths;
};

#endif  // RENDERPARA_H
