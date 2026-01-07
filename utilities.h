#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <random>

using Color = glm::vec3;
using u8Color = glm::u8vec3;

inline int RandomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

inline float RandomFloat() {
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    static std::mt19937 gen;
    return dist(gen);
}

inline float RandomFloat(float min, float max) {
    return RandomFloat() * (max - min) + min;
}

inline glm::vec3 RandomVec3() {
    return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline glm::vec3 RandomVec3(float min, float max) {
    return (RandomVec3() * (max-min)) + min;
}

inline glm::vec3 RandomUnitVector() {
    while (true) {
        glm::vec3 point = RandomVec3(-1.0f, 1.0f);
        float lenSquared = glm::dot(point, point);
        if (lenSquared > 1e-45 && lenSquared <= 1.0f) {
            return point / std::sqrt(lenSquared);
        }
    }
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal) {
    while (true) {
        glm::vec3 point = RandomUnitVector();
        if (glm::dot(point, normal) > 0.0f) {
            return point;
        }
    }
}

inline glm::vec3 RandomOnUnitCircle() {
    while (true) {
        glm::vec3 point = glm::vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f);
        if (glm::dot(point, point) <= 1.0f) {
            return point;
        }
    }
}

inline bool NearZero(const glm::vec3& v) {
    float e = 1e-8;
    return std::fabs(v[0]) < e && std::fabs(v[1]) < e && std::fabs(v[2]) < e;
}
