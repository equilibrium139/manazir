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
        float u = p.x - std::floor(p.x);
        float v = p.y - std::floor(p.y);
        float w = p.z - std::floor(p.z);

        int i = int(std::floor(p.x));
        int j = int(std::floor(p.y));
        int k = int(std::floor(p.z));

        float c[2][2][2];
        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = randFloats[
                        permX[(i+di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255]
                    ];
                }
            }
        }

        return TrilinearInterp(c, u, v, w);
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

    static float TrilinearInterp(float c[2][2][2], float u, float v, float w) {
        float accum = 0.0f;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    accum += (i*u + (1-i)*(1-u))
                            *(j*v + (1-j)*(1-v))
                            *(k*w + (1-k)*(1-w))
                            *c[i][j][k];
                }
            }
        }
        return accum;
    }
};
