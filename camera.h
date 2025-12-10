#pragma once

#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "stb_image_write.h"
#include <cmath>
#include <random>
#include "utilities.h"

class Camera {
public:
    float imAspect = 16.0f / 9.0f;
    int imWidth = 400;
    int samplesPerPixel = 100;
    int maxDepth = 50;
    float vfov = 20.0f;
    float defocusAngle = 0.6f;
    float focalDistance = 10.0f;
    glm::vec3 cameraPos = {13.0f, 2.0f, 3.0f};
    glm::vec3 lookAt = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
    // returns <0 for no intersect, >=0 otherwise
    Color ComputeRayColor(const Ray& ray, int depth, const HittableList& world) {
        if (depth <= 0) return Color(0.0f);
        HitRecord record;
        if (Hit(world, ray, 0.001f, INFINITY, record)) {
            Ray scattered;
            Color attenuation;
            if (record.material->Scatter(ray, record, attenuation, scattered)) {
                return attenuation * ComputeRayColor(scattered, depth - 1, world) ;
            }
            else return Color(0.0f);
        }
        else { 
            constexpr glm::vec3 blue = { 0.5f, 0.7f, 1.0f };
            constexpr glm::vec3 white = { 1.0f, 1.0f, 1.0f };
            const glm::vec3 rayDirNormalized = glm::normalize(ray.direction);
            const float yNormalized = rayDirNormalized.y * 0.5f + 0.5f;
            const float t = yNormalized;
            return white + (blue - white) * t;
        }
    }

    std::vector<glm::u8vec3> Render(const HittableList& world) {
        Initialize();
        std::vector<Color> pixels(imWidth*imHeight, glm::vec3(0.0f));
        for (int row = 0; row < imHeight; row++) {
            for (int col = 0; col < imWidth; col++) {
                glm::vec3 viewportPointCenter = topLeftPixel + ((float)row * pixelDeltaY) + ((float)col * pixelDeltaX);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    float xOffsetFactor = RandomFloat(-0.5f, 0.5f);
                    float yOffsetFactor = RandomFloat(-0.5f, 0.5f);
                    glm::vec3 samplePoint = viewportPointCenter + pixelDeltaX * xOffsetFactor + pixelDeltaY * yOffsetFactor;
                    glm::vec3 rayOrigin = defocusAngle <= 0.0f ? cameraPos : DefocusDiskSample();
                    Ray ray = { .origin = rayOrigin, .direction = samplePoint - rayOrigin, .time = RandomFloat() };
                    pixels[row*imWidth+col] += ComputeRayColor(ray, maxDepth, world);
                }
            }
        }
        std::vector<glm::u8vec3> image(imWidth*imHeight);
        for (int i = 0; i < pixels.size(); i++) {
            auto pixel = pixels[i] * pixelSamplesScale;
            pixel = glm::sqrt(pixel); // gamma correct
            pixel = glm::clamp(pixel, 0.0f, 0.999f);
            image[i] = pixel * 256.0f;
        }
        stbi_write_png("manazir.png", imWidth, imHeight, 3, image.data(), imWidth * 3);
        return image;
    }

private:
    int imHeight;
    glm::vec3 pixelDeltaX;
    glm::vec3 pixelDeltaY;
    glm::vec3 topLeftPixel;
    float pixelSamplesScale;
    glm::vec3 u, v, w;
    glm::vec3 defocusDiskRight;
    glm::vec3 defocusDiskUp;

    void Initialize() {
        imHeight = imWidth / imAspect;
        const float h = std::tan(glm::radians(vfov/2.0f));
        float viewportHeight = 2.0f * h * focalDistance;
        float viewportWidth = viewportHeight * ((float)imWidth / imHeight);
        w = glm::normalize(cameraPos - lookAt);
        u = glm::normalize(glm::cross(up, w));
        v = glm::normalize(glm::cross(w, u));
        float pixelWidth = viewportWidth / imWidth;
        float pixelHeight = viewportHeight / imHeight;
        pixelDeltaX = pixelWidth * u;
        pixelDeltaY = -pixelHeight * v;
        glm::vec3 viewportCenter = cameraPos - focalDistance * w;
        glm::vec3 viewportTopLeft = viewportCenter - (viewportWidth / 2.0f) * u + (viewportHeight / 2.0f) * v;
        topLeftPixel = viewportTopLeft + 0.5f * (pixelDeltaX + pixelDeltaY);
        pixelSamplesScale = 1.0f / samplesPerPixel;
        float defocusRadius = std::tan(glm::radians(defocusAngle / 2.0f)) * focalDistance;
        defocusDiskRight = u * defocusRadius;
        defocusDiskUp = v * defocusRadius;
    }

    glm::vec3 DefocusDiskSample() const {
        glm::vec3 randomPoint = RandomOnUnitCircle();
        return cameraPos + randomPoint.x * defocusDiskRight + randomPoint.y * defocusDiskUp;
    }
};
