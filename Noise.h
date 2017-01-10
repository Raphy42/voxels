//
// Created by Raphaël Dantzer on 10/01/17.
//

#ifndef VOXELS_NOISE_H
#define VOXELS_NOISE_H

#include <glm/common.hpp>
#include <glm/gtc/noise.hpp>

namespace noise {
static float noise2d(float x, float y, int seed, int octaves, float persistence) {
    float   sum = 0,
            strength = 1.0,
            scale = 1.0;
    for(int i = 0; i < octaves; i++) {
        sum += strength * glm::simplex(glm::vec2(x, y) * scale);
        scale *= 2.0;
        strength *= persistence;
    }
    return sum;
}

static float noise3d_abs(float x, float y, float z,int seed, int octaves, float persistence) {
    float   sum = 0,
            strength = 1.0,
            scale = 1.0;
    for(int i = 0; i < octaves; i++) {
        sum += strength * fabs(glm::simplex(glm::vec3(x, y, z) * scale));
        scale *= 2.0;
        strength *= persistence;
    }
    return sum;
}

}

#endif //VOXELS_NOISE_H
