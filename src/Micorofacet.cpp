#include "Micorofacet.h"

Vector3d Micorofacet::eval(Vector3d wi_dir, Vector3d wo_dir, const Hit_record& rec) const {
    // *l* stands for wi_dir, *v* stands for *wo_dir*
    Vector3d unit_wi_dir = wi_dir.normalized();
    Vector3d unit_wo_dir = (-wo_dir).normalized();
    Vector3d normal = rec.normal;
    double cos_n_wi = normal.dot(unit_wi_dir);
    if(cos_n_wi <= 0.0) {
        return Vector3d{0.0, 0.0, 0.0};
    }

    Vector3d h = (unit_wi_dir + unit_wo_dir) / (unit_wi_dir + unit_wo_dir).norm();

    double u = rec.u;
    double v = rec.v;
    Vector3d point = rec.p;
    Vector3d kd = _kd->value(u, v, point);
    Vector3d ks = _ks->value(u, v, point);
    Vector3d albedo = _albedo->value(u, v, point);
    Vector3d roughness = _roughness->value(u, v, point);

    // NDF
    Vector3d alpha = roughness.array().square();
    Vector3d alpha_2 = alpha.array().square();
    double cos_n_h = normal.dot(h);
    Vector3d denominator =
        pi * (std::pow(cos_n_h, 2) * (alpha_2 - Vector3d{1.0, 1.0, 1.0}) + Vector3d{1.0, 1.0, 1.0}).array().square();
    Vector3d D = alpha_2.cwiseQuotient(denominator);

    // Geometry function
    Vector3d k = (roughness + Vector3d{1.0, 1.0, 1.0}).array().square() / 8;

    double cos_n_wo = normal.dot(unit_wo_dir);
    Vector3d G1_wi = Vector3d{cos_n_wi, cos_n_wi, cos_n_wi}.cwiseQuotient(cos_n_wi * (Vector3d{1.0, 1.0, 1.0} - k) + k);
    Vector3d G1_wo = Vector3d{cos_n_wo, cos_n_wo, cos_n_wo}.cwiseQuotient(cos_n_wo * (Vector3d{1.0, 1.0, 1.0} - k) + k);
    Vector3d G = G1_wi.cwiseProduct(G1_wo);

    // Fresnel Equation
    double eta_over_etap = (rec.front_face) ? (1 / _index_of_refraction) : (_index_of_refraction);
    double r0 = (eta_over_etap - 1) / (eta_over_etap + 1);
    r0 = r0 * r0;
    double F = r0 + (1 - r0) * std::pow((1 - cos_n_wo), 5);

    // BRDF
    Vector3d diffuse = kd.cwiseProduct(albedo / pi);
    Vector3d specular = F * G.cwiseProduct(D) / (4 * cos_n_wi * cos_n_wo);
    return kd.cwiseProduct(albedo / pi) + ks.cwiseProduct(specular);
}
