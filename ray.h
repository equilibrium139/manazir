#pragma once

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin, direction;
    float time;
    glm::vec3 PointAt(float t) const {
        return origin + direction * t;
    }
};

