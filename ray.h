#pragma once

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin, direction;
    glm::vec3 PointAt(float t) {
        return origin + direction * t;
    }
};

