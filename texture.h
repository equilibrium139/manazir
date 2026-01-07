#pragma once

#include <algorithm>
#include <glm/vec3.hpp>
#include "image.h"
#include "utilities.h"
#include <memory>

class Texture {
public:
    virtual ~Texture() = default;
    virtual Color Value(float u, float v, const glm::vec3& point) const = 0;
};

class ColorTexture : public Texture {
public:
    ColorTexture(const Color& albedo)
        :albedo(albedo) {}
    Color Value(float u, float v, const glm::vec3& point) const override {
        return albedo;
    }
private:
    Color albedo;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(const Color& c1, const Color& c2)
        :t1(std::make_shared<ColorTexture>(c1)), t2(std::make_shared<ColorTexture>(c2)) {}
    CheckerTexture(std::shared_ptr<Texture> t1, std::shared_ptr<Texture> t2)
        :t1(t1), t2(t2) {}
    Color Value(float u, float v, const glm::vec3& point) const override {
        const int sum = std::floor(point.x) + std::floor(point.y) + std::floor(point.z);
        const int texIdx = sum % 2;
        return texIdx == 0 ? t1->Value(u, v, point) : t2->Value(u, v, point);
    }
private:
    std::shared_ptr<Texture> t1;
    std::shared_ptr<Texture> t2;
};

class ImageTexture : public Texture {
public:
    ImageTexture(const char* imgFile) 
        : image(imgFile) {}
    Color Value(float u, float v, const glm::vec3 &point) const override {
        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);
        int x = int(u * (image.width - 1));
        int y = int(v * (image.height - 1));
        auto pixel = image.PixelAt(x, y);
        constexpr float colorScale = 1.0f / 255.0f;
        return pixel * colorScale;
    }
private:
    Image image;
};
