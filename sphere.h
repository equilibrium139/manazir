#pragma once

#include "hittable.h"
#include <memory>

struct Sphere : public Hittable {
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> material;
    
    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) 
        :center(center), radius(radius), material(std::move(material)) {};

    virtual bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& rec) const override {
        const float a = glm::dot(ray.direction, ray.direction);
        const float b = 2 * glm::dot(ray.direction, ray.origin - center);
        const glm::vec3 rayToSphere = center - ray.origin;
        const float c = glm::dot(rayToSphere, rayToSphere) - radius * radius;
        const float discriminant = b*b - 4*a*c;
        if (discriminant < 0.0f) return false;

        float sqrtd = std::sqrt(discriminant);
        float root = (-b - sqrtd) / (2.0f*a);
        if (root <= tMin || root >= tMax) {
            root = (-b + sqrtd) / (2.0f*a);
            if (root <= tMin || root >= tMax) {
                return false;
            }
        }
        rec.t = root;
        rec.point = ray.origin + rec.t * ray.direction;
        rec.normal = (rec.point - center) / radius;
        rec.material = material;

        if (glm::dot(rec.normal, ray.direction) > 0.0f) {
            rec.normal = -rec.normal;
            rec.frontFace = false;
        }
        else rec.frontFace = true;

        return true;
    }
};
