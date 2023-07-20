#ifndef MYMATH_H
#define MYMATH_H

#include <Dense>
#include <random>

using Vector3d = Eigen::Vector3d;
using Matrix3d = Eigen::Matrix3d;

using Color3d = Vector3d;

// Constants

// *const* objects are valid within the file by default
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

static double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

static int random_int(int min, int max) {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

static double random_double() {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);  // number range is left closed and right open
    return distribution(generator);
}

static double random_double(double min, double max) {
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

static Vector3d random_vec3d(double min, double max) {
    return {random_double(min, max), random_double(min, max), random_double(min, max)};
}

static Vector3d random_in_unit_sphere() {
    while (true) {
        Vector3d v = random_vec3d(0.0, 1.0);
        if (v.dot(v) <= 1.0) {
            return v;
        }
    }
}

static Vector3d random_in_unit_disk() {
    while (true) {
        Vector3d v{random_double(0.0, 1.0), random_double(0.0, 1.0), 0.0};
        if (v.dot(v) <= 1.0) {
            return v;
        }
    }
}

static Vector3d random_unit_vector() { return random_in_unit_sphere().normalized(); }

static Vector3d random_in_hemisphere(const Vector3d& normal) {
    auto in_unit_sphere = random_in_unit_sphere();
    if (normal.dot(in_unit_sphere) > 0) {
        return in_unit_sphere;
    } else {
        return -in_unit_sphere;
    }
}

static bool near_zero(Vector3d vec) {
    const auto a = 1e-8;
    if ((vec.x() < a) && (vec.y() < a) && (vec.z() < a)) {
        return true;
    }

    return false;
}

static Vector3d reflect(Vector3d vec_in, Vector3d normal) { return vec_in - 2 * vec_in.dot(normal) * normal; }

// Attention: *vec_in* is unit vector
static Vector3d refract(Vector3d vec_in, Vector3d normal, double eta_over_etap) {
    Vector3d vec_out_perpendicular = eta_over_etap * (vec_in - vec_in.dot(normal) * normal);
    Vector3d vec_out_parallel = -std::sqrt(1 - vec_out_perpendicular.dot(vec_out_perpendicular)) * normal;

    return vec_out_perpendicular + vec_out_parallel;
}

#endif  // MYMATH_H
