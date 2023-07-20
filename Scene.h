#ifndef SCENE_H
#define SCENE_H

#include "Sphere.h"
#include "Texture.h"
#include "Hittable_list.h"
#include "Light.h"
#include "Rect.h"

static Hittable_list random_scene() {
    Hittable_list world;

    //    auto ground_material = std::make_shared<Lambertian>(Color3d(0.5, 0.5, 0.5));
    //    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, ground_material));

    auto checker = std::make_shared<Check_Texture>(Color3d{0.2, 0.3, 0.1}, Color3d{0.9, 0.9, 0.9});
    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

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
                    //                    auto center2 = center + Vector3d{0, random_double(0.0, 0.5), 0};
                    //                    world.add(std::make_shared<Moving_Sphere>(center, 0.0, center2, 1.0, 0.2,
                    //                    sphere_material));
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

static Hittable_list two_sphere() {
    Hittable_list world;

    auto checker = std::make_shared<Check_Texture>(Color3d{0.2, 0.3, 0.1}, Color3d{0.9, 0.9, 0.9});

    world.add(std::make_shared<Sphere>(Vector3d(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Vector3d(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    return world;
}

static Hittable_list two_perlin_sphere() {
    Hittable_list world;

    auto perlin_texture = std::make_shared<Perlin_Texture>(4);

    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, std::make_shared<Lambertian>(perlin_texture)));

    return world;
}

static Hittable_list earth() {
    auto earth_texture = std::make_shared<Image_Texture>("E:\\projects\\mytinyraytracer\\images\\earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Vector3d(0,0,0), 2, earth_surface);

    return Hittable_list(globe);
}

static Hittable_list simple_light() {
    Hittable_list world;

    auto perlin_texture = std::make_shared<Perlin_Texture>(4);

    world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, std::make_shared<Lambertian>(perlin_texture)));

    auto light_texture = std::make_shared<Diffuse_light>(Color3d{4.0,4.0,4.0});
    world.add(std::make_shared<XY_Rect>(3, 5, 1, 3, -2, light_texture));

    return world;
}

static Hittable_list cornell_box() {
    Hittable_list world;

    auto red   = std::make_shared<Lambertian>(Color3d(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color3d(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color3d(.12, .45, .15));
    auto light = std::make_shared<Diffuse_light>(Color3d(15, 15, 15));

    world.add(std::make_shared<YZ_Rect>(0, 555, 0, 555, 555, green));
    world.add(std::make_shared<YZ_Rect>(0, 555, 0, 555, 0, red));
    world.add(std::make_shared<XZ_Rect>(213, 343, 227, 332, 554, light));
    world.add(std::make_shared<XZ_Rect>(0, 555, 0, 555, 0, white));
    world.add(std::make_shared<XZ_Rect>(0, 555, 0, 555, 555, white));
    world.add(std::make_shared<XY_Rect>(0, 555, 0, 555, 555, white));

    return world;
}

#endif // SCENE_H
