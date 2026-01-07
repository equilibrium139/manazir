#pragma once

#include "glm/geometric.hpp"
#include "hittable.h"
#include "texture.h"
#include "utilities.h"
#include <memory>

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
        :albedo(std::make_shared<ColorTexture>(albedo)) {}
    Lambertian(std::shared_ptr<Texture> tex) 
        : albedo(tex) {}
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const override {
        attenuation = albedo->Value(hitRec.uv.x, hitRec.uv.y, hitRec.point);
        scattered = { .origin = hitRec.point, .direction = hitRec.normal + RandomUnitVector(), .time=incident.time };
        if (NearZero(scattered.direction)) scattered.direction = hitRec.normal;
        return true;
    }
    std::shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
    Metal(Color albedo, float fuzz = 0.0f)
        :albedo(albedo), fuzz(fuzz > 1.0f ? 1.0f : fuzz) {}
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const override {
        attenuation = albedo;
        glm::vec3 reflected = glm::normalize(glm::reflect(incident.direction, hitRec.normal)) + RandomUnitVector() * fuzz;
        scattered = { .origin = hitRec.point, .direction = reflected, .time=incident.time };
        return glm::dot(scattered.direction, hitRec.normal) > 0.0f;
    }
    Color albedo;
    float fuzz;
};


class Dielectric : public Material {
public:
    Dielectric(float refractiveIndex) : refractiveIndex(refractiveIndex) {}
    virtual bool Scatter(const Ray& incident, const HitRecord& hitRec, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0f);
        scattered.origin = hitRec.point;
        scattered.time = incident.time;
        const glm::vec3 incidentDirNormalized = glm::normalize(incident.direction);
        float cosTheta = std::fmin(dot(-incidentDirNormalized, hitRec.normal), 1.0f);
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
        // The third argument of refract is the ratio of source index to destination index. So if the face is a front face,
        // that means we're going from air (1.0) to the material (refractiveIndex), hence 1.0/refractiveIndex
        float refractiveRatio = hitRec.frontFace ? 1.0f / refractiveIndex : refractiveIndex;
        bool cannotRefract = sinTheta * refractiveRatio > 1.0f;
        if (cannotRefract || Reflectance(cosTheta, refractiveRatio) > RandomFloat()) {
            scattered.direction = glm::reflect(incidentDirNormalized, hitRec.normal);
        }
        else {
            scattered.direction = glm::refract(incidentDirNormalized, hitRec.normal, refractiveRatio);
        }
        return true;
    }
    float refractiveIndex;
private:
    static float Reflectance(float cosine, float refractiveIndex) {
        float r0 = (1-refractiveIndex) / (1+refractiveIndex);
        r0 = r0 * r0;
        return r0 + (1-r0)*std::pow((1 - cosine), 5);
    }
};
