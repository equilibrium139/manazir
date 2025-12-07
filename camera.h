#pragma once

#include "hittable.h"
#include "ray.h"
#include "stb_image_write.h"
#include <random>
#include "utilities.h"

using Color = glm::vec3;

class Camera {
public:
    float imAspect = 16.0f / 9.0f;
    int imWidth = 400;
    int samplesPerPixel = 100;
    int maxDepth = 50;
    // returns <0 for no intersect, >=0 otherwise
    Color ComputeRayColor(const Ray& ray, int depth, const HittableList& world) {
        if (depth <= 0) return Color(0.0f);
        HitRecord record;
        if (Hit(world, ray, 0.001f, INFINITY, record)) {
            glm::vec3 bounceDir = record.normal + RandomUnitVector();
            Ray bounce = { .origin = record.point, .direction = bounceDir };
            return 0.1f * ComputeRayColor(bounce, depth - 1, world) ;
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
                    Ray ray = { .origin = cameraPos, .direction = samplePoint - cameraPos };
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
    glm::vec3 cameraPos;
    glm::vec3 pixelDeltaX;
    glm::vec3 pixelDeltaY;
    glm::vec3 topLeftPixel;
    float pixelSamplesScale;

    void Initialize() {
        imHeight = imWidth / imAspect;
        cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
        float focalDistance = 1.0f;
        float viewportHeight = 2.0f;
        float viewportWidth = viewportHeight * ((float)imWidth / imHeight);
        float pixelWidth = viewportWidth / imWidth;
        float pixelHeight = viewportHeight / imHeight;
        pixelDeltaX = glm::vec3(pixelWidth, 0.0f, 0.0f);
        pixelDeltaY = glm::vec3(0.0f, -pixelHeight, 0.0f);
        glm::vec3 viewportCenter = cameraPos - glm::vec3(0.0f, 0.0f, focalDistance);
        glm::vec3 viewportTopLeft = viewportCenter - glm::vec3(viewportWidth / 2.0f, -viewportHeight / 2.0f, 0.0f);
        topLeftPixel = viewportTopLeft + 0.5f * (pixelDeltaX + pixelDeltaY);
        pixelSamplesScale = 1.0f / samplesPerPixel;
    }
};
