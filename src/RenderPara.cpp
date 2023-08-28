#include "RenderPara.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include "Light.h"
#include "Material.h"
#include "Micorofacet.h"
#include "Sphere.h"
#include "Triangle.h"

void RenderPara::build_screen(const json& screen) {
    // Screen
    width = screen["width"];
    spdlog::info("width: {}", width);

    aspect_ratio = screen["aspect_ratio"];
    spdlog::info("aspect_ratio: {}", aspect_ratio);

    height = static_cast<int>(width / aspect_ratio);
    canvas = Screen{width, height};
}

void RenderPara::build_camera(const json& camera) {
    // Camera
    std::vector<double> _lookfrom = camera["lookfrom"];
    lookfrom << _lookfrom[0], _lookfrom[1], _lookfrom[2];
    spdlog::info("lookfrom: [ {} ]", Vec3d_to_str(lookfrom));

    std::vector<double> _lookat = camera["lookat"];
    lookat << _lookat[0], _lookat[1], _lookat[2];
    spdlog::info("lookat: [ {} ]", Vec3d_to_str(lookat));

    std::vector<double> _vup = camera["vup"];
    vup << _vup[0], _vup[1], _vup[2];
    spdlog::info("vup: [ {} ]", Vec3d_to_str(vup));

    vfov = camera["vfov"];
    spdlog::info("vfov: {}", vfov);

    aperture = camera["aperture"];
    spdlog::info("aperture: {}", aperture);

    focus_dist = camera["focus_dist"];
    spdlog::info("focus_dist: {}", focus_dist);

    cam = Camera{lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, 0.0, 0.0};
}

std::unordered_map<std::string, RenderPara::Texture_Type> RenderPara::t = {{"Solid_Texture", t_Solid_Texture},
                                                                           {"Image_Texture", t_Image_Texture}};

std::shared_ptr<Texture> RenderPara::build_texture(const json& texture) const {
    auto type = texture["texture_type"];

    std::shared_ptr<Texture> tex_ptr;
    switch (t[type]) {
        case t_Solid_Texture: {
            auto color_vector = texture["color"];
            Color3d color;
            color << color_vector[0], color_vector[1], color_vector[2];
            tex_ptr = std::make_shared<Solid_Texture>(color);

            spdlog::info("{} color is [ {} ]", type, Vec3d_to_str(color));
        } break;
        case t_Image_Texture: {
            std::string img_path = texture["image_path"];
            tex_ptr = std::make_shared<Image_Texture>(img_path.c_str());

            spdlog::info("{} img_path is {}", type, img_path);
        }
        default:
            break;
    }

    return tex_ptr;
}

std::unordered_map<std::string, RenderPara::Material_Type> RenderPara::m = {{"Metal", m_Metal},
                                                                            {"Lambertian", m_Lambertian},
                                                                            {"Diffuse_light", m_Diffuse_light},
                                                                            {"Micorofacet", m_Micorofacet}};

std::shared_ptr<Material> RenderPara::build_material(const json& material) const {
    auto type = material["material_type"];

    std::shared_ptr<Material> mat_ptr;
    switch (m[type]) {
        case m_Metal: {
            auto texture_type = material["albedo"]["texture_type"];
            spdlog::info("{} albedo is {}.", type, texture_type);
            auto albedo_tex_ptr = build_texture(material["albedo"]);
            double fuzzy = material["fuzzy"];
            mat_ptr = std::make_shared<Metal>(albedo_tex_ptr, fuzzy);
        } break;
        case m_Lambertian: {
            auto texture_type = material["albedo"]["texture_type"];
            spdlog::info("{} albedo is {}.", type, texture_type);
            auto albedo_tex_ptr = build_texture(material["albedo"]);
            mat_ptr = std::make_shared<Lambertian>(albedo_tex_ptr);
        } break;
        case m_Diffuse_light: {
            auto texture_type = material["emit"]["texture_type"];
            spdlog::info("{} emit is {}.", type, texture_type);
            auto albedo_tex_ptr = build_texture(material["emit"]);
            mat_ptr = std::make_shared<Diffuse_light>(albedo_tex_ptr);
        } break;
        case m_Micorofacet: {
            double index_of_refraction = material["index_of_refraction"];
            spdlog::info("{}'s index_of_refraction is {}", type, index_of_refraction);

            auto kd_texture_type = material["kd"]["texture_type"];
            spdlog::info("{} kd is {}", type, kd_texture_type);
            auto kd_tex_ptr = build_texture(material["kd"]);

            auto ks_texture_type = material["ks"]["texture_type"];
            spdlog::info("{} ks is {}", type, ks_texture_type);
            auto ks_tex_ptr = build_texture(material["ks"]);

            auto albedo_texture_type = material["albedo"]["texture_type"];
            spdlog::info("{} albedo is {}", type, albedo_texture_type);
            auto albedo_tex_ptr = build_texture(material["albedo"]);

            auto roughness_texture_type = material["roughness"]["texture_type"];
            spdlog::info("{} roughness is {}", type, roughness_texture_type);
            auto roughness_tex_ptr = build_texture(material["roughness"]);

            mat_ptr = std::make_shared<Micorofacet>(index_of_refraction, kd_tex_ptr, ks_tex_ptr, albedo_tex_ptr,
                                                    roughness_tex_ptr);
        } break;
        default:
            spdlog::error("Invalid material type {}", type);
            exit(1);
            break;
    }

    return mat_ptr;
}

void RenderPara::build_obj(const std::string& obj_path, std::shared_ptr<Material> mat_ptr) {
    // TODO: Need to add the ability to process mtl files
    std::string inputfile = obj_path;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";  // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            spdlog::error("TinyObjReader: {}", reader.Error());
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        spdlog::error("TinyObjReader: {}", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    spdlog::info("{} OBJ shapes's size is {}", obj_path, shapes.size());
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        spdlog::info("{} OBJ shapes[{}]'s size is {}", obj_path, s, shapes[s].mesh.num_face_vertices.size());
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (fv != 3) {
                spdlog::error("it is not a triangle in {}!", obj_path);
                exit(1);
            }

            // Loop over vertices in the face.
            std::vector<Vector3d> points(3);
            std::vector<Vector3d> normals(3);
            bool no_normal = true;  // Attention: if OBJ file doesn't have normals, we should calculate normals.
            std::vector<Vector2d> uvs(3);
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                points[v] << vx, vy, vz;

                // Check if `normal_index` is zero or positive. negative = no normal
                // data
                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    no_normal = false;
                    normals[v] << nx, ny, nz;
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord
                // data
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    uvs[v] << tx, ty;
                }

                // Optional: vertex colors
                // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            // Attention: calculate normals.
            if (no_normal) {
                normals[0] = ((points[1] - points[0]).cross(points[2] - points[0])).normalized();
                normals[1] = normals[0];
                normals[2] = normals[0];
            }
            world.add(std::make_shared<Triangle>(points, normals, uvs, mat_ptr));
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}

void RenderPara::build_world(const json& scene) {
    if (scene.find("objs") != scene.end()) {
        auto objs = scene["objs"];
        for (const auto& obj : objs) {
            std::string obj_path = obj["obj_path"];

            spdlog::info(">>>>>>>>>>>> {} begin building.", obj_path);
            auto material = obj["material"];
            auto material_type = obj["material"]["material_type"];
            spdlog::info("{} material type is {}", obj_path, material_type);

            auto mat_ptr = build_material(material);
            build_obj(obj_path, mat_ptr);
            spdlog::info(">>>>>>>>>>>> {} has successfully built!\n", obj_path);
        }
    }

    if (scene.find("spheres") != scene.end()) {
        auto spheres = scene["spheres"];
        for (const auto& sphere : spheres) {
            auto origin_vector = sphere["origin"];
            Vector3d origin;
            origin << origin_vector[0], origin_vector[1], origin_vector[2];
            double radius = sphere["radius"];

            spdlog::info(">>>>>>>>>>>> Sphere origin = [ {} ], radius = {} begin building.", Vec3d_to_str(origin),
                         radius);
            auto material = sphere["material"];
            auto material_type = sphere["material"]["material_type"];
            spdlog::info("Sphere origin = [ {} ], radius = {}, material type is {}", Vec3d_to_str(origin), radius,
                         material_type);

            auto mat_ptr = build_material(material);
            world.add(std::make_shared<Sphere>(origin, radius, mat_ptr));
            spdlog::info(">>>>>>>>>>>> Sphere origin = [ {} ], radius = {} has successfully built!\n",
                         Vec3d_to_str(origin), radius);
        }
    }
}
