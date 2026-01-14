#pragma once

#include "utilities.h"
#include <algorithm>

class Perlin {
public:
    Perlin() {
        for (int i = 0; i < pointCount; i++) {
            randVecs[i] = RandomVec3(-1.0f, 1.0f);
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

        glm::vec3 c[2][2][2];
        for (int di = 0; di < 2; di++) {
            for (int dj = 0; dj < 2; dj++) {
                for (int dk = 0; dk < 2; dk++) {
                    c[di][dj][dk] = randVecs[
                        permX[(i+di) & 255] ^
                        permY[(j+dj) & 255] ^
                        permZ[(k+dk) & 255]
                    ];
                }
            }
        }

        return PerlinInterp(c, u, v, w);
    }

    float Turbulence(const glm::vec3& point, int depth) const {
        float accum = 0.0f;
        glm::vec3 tempPoint = point;
        float weight = 1.0f;
        
        for (int i = 0; i < depth; i++) {
            accum += weight * Noise(tempPoint);
            weight *= 0.5f;
            tempPoint *= 2;
        }

        return std::fabs(accum);
    }
private:
    static const int pointCount = 256;
    glm::vec3 randVecs[pointCount];
    int permX[pointCount];
    int permY[pointCount];
    int permZ[pointCount];

    static void GeneratePerm(int* p) {
        for (int i = 0; i < pointCount; i++) {
            p[i] = i;
        }
        std::random_shuffle(p, p + pointCount);
    }

    static float PerlinInterp(glm::vec3 c[2][2][2], float u, float v, float w) {
        float uu = u*u*(3-2*u);
        float vv = v*v*(3-2*v);
        float ww = w*w*(3-2*w);
        float accum = 0.0f;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    glm::vec3 weight(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                            *(j*vv + (1-j)*(1-vv))
                            *(k*ww + (1-k)*(1-ww))
                            *glm::dot(c[i][j][k], weight);
                }
            }
        }
        return accum;
    }
};
