#pragma once

#include <glm/glm.hpp>
#include <random>

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
