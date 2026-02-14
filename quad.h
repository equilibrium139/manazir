#pragma once
#include "hittable.h"

struct Quad : public Hittable {
    Quad(const glm::vec3& corner, const glm::vec3& u, const glm::vec3& v, std::shared_ptr<Material> material)
        : corner(corner), u(u), v(v), aabb(ComputeAABB(*this)), material(material) {
        glm::vec3 n = glm::cross(u, v);
        w = n / glm::dot(n, n);
        normal = glm::normalize(n);
        d = glm::dot(corner, normal);
    }


    bool Hit(const Ray &ray, float tMin, float tMax, HitRecord &rec) const override {
        float denominator = glm::dot(normal, ray.direction);
        if (std::fabs(denominator) < 1e-8) return false;

        float numerator = d - glm::dot(normal, ray.origin);
        float t = numerator / denominator;
        if (t < tMin || t > tMax) return false;
        glm::vec3 intersection = ray.PointAt(t);
        glm::vec3 planarIntersectionVec = intersection - corner;
        float alpha = glm::dot(w, glm::cross(planarIntersectionVec, v));
        float beta = glm::dot(w, glm::cross(u, planarIntersectionVec));
        if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) {
            return false; // ray hits plane but not quad
        }
        rec.uv.x = alpha;
        rec.uv.y = beta;
        rec.t = t;
        rec.point = intersection;
        rec.material = material;
        rec.SetFaceNormal(ray.direction, normal);
        return true;
    }

    AABB GetAABB() const override {
       return aabb;
    }

    glm::vec3 corner, u, v;
    glm::vec3 normal;
    float d;
    AABB aabb;
    std::shared_ptr<Material> material;
    glm::vec3 w;
private:
    static AABB ComputeAABB(const Quad& quad) {
        AABB a = AABB(quad.corner, quad.corner + quad.u + quad.v);
        AABB b = AABB(quad.corner + quad.u, quad.corner + quad.v);
        return AABB(a, b);
    }
};