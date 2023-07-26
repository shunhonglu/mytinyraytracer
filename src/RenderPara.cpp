#include "RenderPara.h"

#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>

#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include "Triangle.h"

std::unordered_map<std::string, RenderPara::Material_Type> RenderPara::m = {{"Lambertian", m_Lambertian},
                                                                            {"Diffuse_light", m_Diffuse_light}};
std::shared_ptr<Material> RenderPara::build_material(const json& material) const {
    auto type = material["material_type"];
    auto color_vector = material["color"];
    Color3d color;
    color << color_vector[0], color_vector[1], color_vector[2];

    std::shared_ptr<Material> mat_ptr;
    switch (m[type]) {
        case m_Lambertian:
            mat_ptr = std::make_shared<Lambertian>(color);
            break;
        case m_Diffuse_light:
            mat_ptr = std::make_shared<Diffuse_light>(color);
            break;
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
            bool no_normal = true;  // if OBJ file doesn't have normals, we should calculate normals.
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
            auto material = obj["material"];
            auto mat_ptr = build_material(material);
            build_obj(obj_path, mat_ptr);
            spdlog::info("{} has been successfully built!", obj_path);
        }
    }

    if (scene.find("spheres") != scene.end()) {
        auto spheres = scene["spheres"];
        for (const auto& sphere : spheres) {
            auto origin_vector = sphere["origin"];
            Vector3d origin;
            origin << origin_vector[0], origin_vector[1], origin_vector[2];
            double radius = sphere["radius"];
            auto material = sphere["material"];
            auto mat_ptr = build_material(material);
            world.add(std::make_shared<Sphere>(origin, radius, mat_ptr));
            spdlog::info("Sphere origin = {}, radius = {} has been successfully built!", Vec3d_to_str(origin), radius);
        }
    }
}
