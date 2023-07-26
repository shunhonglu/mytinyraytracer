#ifndef SCENE_H
#define SCENE_H

#include <functional>
#include <unordered_map>

#include "Hittable_list.h"
#include "Light.h"
#include "Sphere.h"
#include "Texture.h"
#include "Triangle.h"

Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = std::make_shared<Lambertian>(Color3d(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vector3d center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Vector3d(4, 0.2, 0)).norm() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_vec3d(0.0, 1.0);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_vec3d(0.5, 1.0);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vector3d(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color3d(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Vector3d(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color3d(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Vector3d(4, 1, 0), 1.0, material3));

    return world;
}

Hittable_list two_sphere() {
    Hittable_list world;

    auto checker = std::make_shared<Check_Texture>(Color3d{0.2, 0.3, 0.1}, Color3d{0.9, 0.9, 0.9});

    world.add(std::make_shared<Sphere>(Vector3d(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Vector3d(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return world;
}

Hittable_list two_perlin_sphere() {
    Hittable_list world;

    auto perlin_texture = std::make_shared<Perlin_Texture>(4);

    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, std::make_shared<Lambertian>(perlin_texture)));

    return world;
}

Hittable_list earth() {
    auto earth_texture = std::make_shared<Image_Texture>("E:\\projects\\mytinyraytracer\\images\\earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Vector3d(0, 0, 0), 2, earth_surface);

    return Hittable_list(globe);
}

Hittable_list test_lambertian() {
    Hittable_list world;

    auto light_material = std::make_shared<Diffuse_light>(Color3d{3.0, 3.0, 3.0});
    auto light_triangle =
        std::make_shared<Triangle>(Vector3d{3.0, 10.0, 0.0}, Vector3d{0.0, 10.0, 0.0}, Vector3d{0.0, 10.0, 3.0},
                                   Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0},
                                   Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, light_material);
    world.add(light_triangle);

    auto sphere =
        std::make_shared<Sphere>(Vector3d{1.0, 1.0, 1.0}, 1.0, std::make_shared<Lambertian>(Color3d{0.3, 0.4, 0.7}));
    world.add(sphere);

    auto triangle = std::make_shared<Triangle>(
        Vector3d{10.0, 0.0, 0.0}, Vector3d{0.0, 0.0, 0.0}, Vector3d{0.0, 0.0, 10.0}, Vector3d{0.0, 1.0, 0.0},
        Vector3d{0.0, 1.0, 0.0}, Vector3d{0.0, 1.0, 0.0}, Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0},
        std::make_shared<Lambertian>(Color3d{0.8, 0.8, 0.8}));
    world.add(triangle);

    return world;
}

Hittable_list bunny(const std::vector<std::string>& scene_obj_paths) {
    Hittable_list world;

    for (const auto& scene_obj_path : scene_obj_paths) {
        Hittable_list temp{scene_obj_path, std::make_shared<Dielectric>(1.5)};
        for (const auto& triangle : temp.get_objects()) {
            world.add(triangle);
        }
    }

    return world;
}

Hittable_list cornell_box() {
    Hittable_list world;

    // Light
    auto light_material = std::make_shared<Diffuse_light>(Color3d{15.0, 15.0, 15.0});
    auto light_triangle1 = std::make_shared<Triangle>(
        Vector3d{213.0, 554.0, 227.0}, Vector3d{213.0, 554.0, 332.0}, Vector3d{343.0, 554.0, 227.0},
        Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, light_material);
    auto light_triangle2 = std::make_shared<Triangle>(
        Vector3d{343.0, 554.0, 332.0}, Vector3d{343.0, 554.0, 227.0}, Vector3d{213.0, 554.0, 332.0},
        Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, light_material);
    world.add(light_triangle1);
    world.add(light_triangle2);

    // Red
    auto red = std::make_shared<Lambertian>(Color3d{.65, .05, .05});
    auto red_triangle1 = std::make_shared<Triangle>(
        Vector3d{0.0, 0.0, 0.0}, Vector3d{0.0, 555.0, 0.0}, Vector3d{0.0, 0.0, 555.0},
        Vector3d{1.0, 0.0, 0.0}, Vector3d{1.0, 0.0, 0.0}, Vector3d{1.0, 0.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, red);
    auto red_triangle2 = std::make_shared<Triangle>(
        Vector3d{0.0, 555.0, 555.0}, Vector3d{0.0, 0.0, 555.0}, Vector3d{0.0, 555.0, 0.0},
        Vector3d{1.0, 0.0, 0.0}, Vector3d{1.0, 0.0, 0.0}, Vector3d{1.0, 0.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, red);
    world.add(red_triangle1);
    world.add(red_triangle2);

    // Green
    auto green = std::make_shared<Lambertian>(Color3d{.12, .45, .15});
    auto green_triangle1 = std::make_shared<Triangle>(
        Vector3d{555.0, 0.0, 0.0}, Vector3d{555.0, 555.0, 0.0}, Vector3d{555.0, 0.0, 555.0},
        Vector3d{-1.0, 0.0, 0.0}, Vector3d{-1.0, 0.0, 0.0}, Vector3d{-1.0, 0.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, green);
    auto green_triangle2 = std::make_shared<Triangle>(
        Vector3d{555.0, 555.0, 555.0}, Vector3d{555.0, 0.0, 555.0}, Vector3d{555.0, 555.0, 0.0},
        Vector3d{-1.0, 0.0, 0.0}, Vector3d{-1.0, 0.0, 0.0}, Vector3d{-1.0, 0.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, green);
    world.add(green_triangle1);
    world.add(green_triangle2);

    // White
    auto white = std::make_shared<Lambertian>(Color3d{.73, .73, .73});
    auto white_triangle1 = std::make_shared<Triangle>(
        Vector3d{0.0, 0.0, 0.0}, Vector3d{0.0, 0.0, 555.0}, Vector3d{555.0, 0.0, 0.0},
        Vector3d{0.0, 1.0, 0.0}, Vector3d{0.0, 1.0, 0.0}, Vector3d{0.0, 1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    auto white_triangle2 = std::make_shared<Triangle>(
        Vector3d{555.0, 0.0, 555.0}, Vector3d{555.0, 0.0, 0.0}, Vector3d{0.0, 0.0, 555.0},
        Vector3d{0.0, 1.0, 0.0}, Vector3d{0.0, 1.0, 0.0}, Vector3d{0.0, 1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    world.add(white_triangle1);
    world.add(white_triangle2);

    auto white_triangle3 = std::make_shared<Triangle>(
        Vector3d{0.0, 555.0, 0.0}, Vector3d{0.0, 555.0, 555.0}, Vector3d{555.0, 555.0, 0.0},
        Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    auto white_triangle4 = std::make_shared<Triangle>(
        Vector3d{555.0, 555.0, 555.0}, Vector3d{555.0, 555.0, 0.0}, Vector3d{0.0, 555.0, 555.0},
        Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector3d{0.0, -1.0, 0.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    world.add(white_triangle3);
    world.add(white_triangle4);

    auto white_triangle5 = std::make_shared<Triangle>(
        Vector3d{0.0, 0.0, 555.0}, Vector3d{0.0, 555.0, 555.0}, Vector3d{555.0, 0.0, 555.0},
        Vector3d{0.0, 0.0, -1.0}, Vector3d{0.0, 0.0, -1.0}, Vector3d{0.0, 0.0, -1.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    auto white_triangle6 = std::make_shared<Triangle>(
        Vector3d{555.0, 555.0, 555.0}, Vector3d{555.0, 0.0, 555.0}, Vector3d{0.0, 555.0, 555.0},
        Vector3d{0.0, 0.0, -1.0}, Vector3d{0.0, 0.0, -1.0}, Vector3d{0.0, 0.0, -1.0}, Vector2d{0.0, 0.0},
        Vector2d{0.0, 0.0}, Vector2d{0.0, 0.0}, white);
    world.add(white_triangle5);
    world.add(white_triangle6);

    // Tall Box
    return world;
}

std::function<Hittable_list()> get_scene_function(std::string function_name) {
    static std::unordered_map<std::string, std::function<Hittable_list()>> m = {{"random_scene", random_scene},
                                                                                {"test_lambertian", test_lambertian},
                                                                                {"cornell_box", cornell_box}};

    return m[function_name];
}

std::function<Hittable_list(const std::vector<std::string>&)> get_obj_scene_function(std::string function_name) {
    static std::unordered_map<std::string, std::function<Hittable_list(const std::vector<std::string>& s)>> m = {
        {"bunny", bunny}};

    return m[function_name];
}

#endif  // SCENE_H
