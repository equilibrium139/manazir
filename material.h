#pragma once

#include "glm/geometric.hpp"
#include "hittable.h"
#include "utilities.h"

class Material {
public:
    virtual ~Material() = default;
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(Color albedo)
        :albedo(albedo) {}
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const override {
        attenuation = albedo;
        scattered = { .origin = hitRec.point, .direction = hitRec.normal + RandomUnitVector() };
        if (NearZero(scattered.direction)) scattered.direction = hitRec.normal;
        return true;
    }
    Color albedo;
};

class Metal : public Material {
public:
    Metal(Color albedo, float fuzz = 0.0f)
        :albedo(albedo), fuzz(fuzz > 1.0f ? 1.0f : fuzz) {}
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const override {
        attenuation = albedo;
        glm::vec3 reflected = glm::normalize(glm::reflect(incident.direction, hitRec.normal)) + RandomUnitVector() * fuzz;
        scattered = { .origin = hitRec.point, .direction = reflected };
        return glm::dot(scattered.direction, hitRec.normal) > 0.0f;
    }
    Color albedo;
    float fuzz;
};
