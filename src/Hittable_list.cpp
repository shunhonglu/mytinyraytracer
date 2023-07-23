#include "Hittable_list.h"

#include "Triangle.h"
#include "spdlog/spdlog.h"
#include "tiny_obj_loader.h"

Hittable_list::Hittable_list(const std::string& obj_path, std::shared_ptr<Material> mat_ptr) {
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
    spdlog::info("OBJ shapes's size is {}", shapes.size());
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        spdlog::info("OBJ shapes[{}]'s size is {}", s, shapes[s].mesh.num_face_vertices.size());
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            if (fv != 3) {
                spdlog::error("it is not a triangle!");
                exit(1);
            }

            // Loop over vertices in the face.
            std::vector<Vector3d> points(3);
            std::vector<Vector3d> normals(3);
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
            _objects.push_back(std::make_shared<Triangle>(points, normals, uvs, mat_ptr));
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}

bool Hittable_list::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : _objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool Hittable_list::bounding_box(double time0, double time1, AABB& bounding_box) const {
    if (_objects.empty()) {
        return false;
    }

    AABB temp;
    bool first_hit = true;

    for (const auto& object : _objects) {
        if (!object->bounding_box(time0, time1, temp)) {
            return false;
        }
        bounding_box = first_hit ? temp : AABB::surround_bounding_box(bounding_box, temp);
        first_hit = false;
    }

    return true;
}
