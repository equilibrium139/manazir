#pragma once

#include "utilities.h"
#include <algorithm>

class Perlin {
public:
    Perlin() {
        for (int i = 0; i < pointCount; i++) {
            randFloats[i] = RandomFloat();
        }

        GeneratePerm(permX);
        GeneratePerm(permY);
        GeneratePerm(permZ);
    }

    float Noise(const glm::vec3& p) const {
        int i = int(4 * p.x) & 255;
        int j = int(4 * p.y) & 255;
        int k = int(4 * p.z) & 255;

        return randFloats[permX[i] ^ permY[j] ^ permZ[k]];
    }
private:
    static const int pointCount = 256;
    float randFloats[pointCount];
    int permX[pointCount];
    int permY[pointCount];
    int permZ[pointCount];

    static void GeneratePerm(int* p) {
        for (int i = 0; i < pointCount; i++) {
            p[i] = i;
        }
        std::random_shuffle(p, p + pointCount);
    }
};
