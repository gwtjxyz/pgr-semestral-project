//----------------------------------------------------------------------------------------
/**
 * \file       terrain.cpp
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include "terrain.h"
#include "glm/glm.hpp"

#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>

// TODO make this less hardcoded and more adaptable to terrains of various sizes
Perlin::Perlin() {
    // from reference implementation
    mPermutation = {
            151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
            8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
            35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
            134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
            55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
            18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
            250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
            189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
            43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
            97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
            107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
    // duplicate
    mPermutation.insert(mPermutation.end(), mPermutation.begin(), mPermutation.end());
}

Perlin::Perlin(unsigned int seed) {
    // set initial array size
    mPermutation.resize(256);
    // fill with sequential values
    std::iota(mPermutation.begin(), mPermutation.end(), 0);
    // reshuffle values based on seed
    std::default_random_engine engine(seed);
    std::shuffle(mPermutation.begin(), mPermutation.end(), engine);
    // duplicate
    mPermutation.insert(mPermutation.end(), mPermutation.begin(), mPermutation.end());
}

double Perlin::noise(double x, double y, double z) {
    // find the unit cube that contains the point
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    // find relative xyz of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // compute fade curves
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    // hash coordinates of the 8 cube corners
    int A  = mPermutation[X]     + Y;
    int AA = mPermutation[A]     + Z;
    int AB = mPermutation[A + 1] + Z;
    int B  = mPermutation[X + 1] + Y;
    int BA = mPermutation[B]     + Z;
    int BB = mPermutation[B + 1] + Z;

    // add blended results from 8 corners of cube
    double res = lerp(w,
                      lerp(v,
                           lerp(u, grad(mPermutation[AA], x, y, z), grad(mPermutation[BA], x-1, y, z)),
                           lerp(u, grad(mPermutation[AB], x, y-1, z), grad(mPermutation[BB], x-1, y-1, z))),
                      lerp(v,
                           lerp(u, grad(mPermutation[AA+1], x, y, z-1), grad(mPermutation[BA+1], x-1, y, z-1)),
                           lerp(u, grad(mPermutation[AB+1], x, y-1, z-1), grad(mPermutation[BB+1], x-1, y-1, z-1))));

    return (res + 1.0) / 2.0;
}

double Perlin::fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin::lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double Perlin::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // convert lower 4 bits of hash into 12 gradient direcitons
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

// helper function for mesh generation
// could be optimized to be better/faster/more efficient
glm::vec3 calculateNormals(int i, int j, int size, float **& mesh) {
    int stride = 8;
    glm::vec3 result = glm::vec3(0.0f);
    // I don't know how to describe what I'm doing with words,
    // so I'll just link this picture here: https://i.imgur.com/c9309UB.png
    // this does a bunch of unnecessary calculations and can definitely be optimized
    // to work faster, but we only run this once and the additional
    // calculations themselves aren't too heavy, so it doesn't matter too much.
    if (i > 0 && j > 0) {
        // 1 x 2
        glm::vec3 vecTop = glm::vec3(mesh[j - 1][i * stride],
                                   mesh[j - 1][i * stride + 1],
                                   mesh[j - 1][i * stride + 2]);
        glm::vec3 vecLeft = glm::vec3(mesh[j][(i - 1) * stride],
                                      mesh[j][(i - 1) * stride + 1],
                                      mesh[j][(i - 1) * stride + 2]);
        result += glm::cross(vecTop, vecLeft);
    }
    if (i > 0 && j < size - 1) {
        // 2 x 3
        glm::vec3 vecLeft = glm::vec3(mesh[j][(i - 1) * stride],
                                      mesh[j][(i - 1) * stride + 1],
                                      mesh[j][(i - 1) * stride + 2]);
        glm::vec3 vecBottomLeft = glm::vec3(mesh[j + 1][(i - 1) * stride],
                                            mesh[j + 1][(i - 1) * stride + 1],
                                            mesh[j + 1][(i - 1) * stride + 2]);
        result += glm::cross(vecLeft, vecBottomLeft);
        // 3 x 4
        // don't have to do a check for the next scenario
        // if the previous one exists so does this one and vice-versa
        glm::vec3 vecBottom = glm::vec3(mesh[j + 1][i * stride],
                                        mesh[j + 1][i * stride + 1],
                                        mesh[j + 1][i * stride + 2]);
        result += glm::cross(vecBottomLeft, vecBottom);
    }
    if (i < size - 1 && j < size - 1) {
        // 4 x 5
        glm::vec3 vecBottom = glm::vec3(mesh[j + 1][i * stride],
                                        mesh[j + 1][i * stride + 1],
                                        mesh[j + 1][i * stride + 2]);
        glm::vec3 vecRight = glm::vec3(mesh[j][(i + 1) * stride],
                                       mesh[j][(i + 1) * stride + 1],
                                       mesh[j][(i + 1) * stride + 2]);
        result += glm::cross(vecBottom, vecRight);
    }
    if (i < size - 1 && j > 0) {
        // 6 x 1 and 5 x 6
        // same reasoning as 2 x 3 and 3 x 4
        glm::vec3 vecRight = glm::vec3(mesh[j][(i + 1) * stride],
                                       mesh[j][(i + 1) * stride + 1],
                                       mesh[j][(i + 1) * stride + 2]);
        glm::vec3 vecTopRight = glm::vec3(mesh[j - 1][(i + 1) * stride],
                                          mesh[j - 1][(i + 1) * stride + 1],
                                          mesh[j - 1][(i + 1) * stride + 2]);
        result += glm::cross(vecRight, vecTopRight);

        glm::vec3 vecTop = glm::vec3(mesh[j - 1][i * stride],
                                     mesh[j - 1][i * stride + 1],
                                     mesh[j - 1][i * stride + 2]);
        result += glm::cross(vecTopRight, vecRight);
    }
    return glm::normalize(result);
}

// generates square terrain with dimensions of n * n, where n == size
// repeat = after how many vertices to start repeating textures
// (if set to zero, we do one biiiiiiiig stretch)
float ** generateTerrain(int size, int repeat) {
    // size == n; we need 8 floats for each vertex
    // 3 for xyz, 3 for normal vector, 2 for texture
    int normalOffset = 3, textureOffset = 6, stride = 8;
    float ** mesh = new float*[size];
    for (int i = 0; i != size; ++i)
        mesh[i] = new float[size * 8];
    // first, fill up the vertex coordinates; X,Z are on a grid, Y we get from Perlin noise
    Perlin noiseGenerator;
    for (int j = 0; j != size; ++j) {       // j == rows
        for (int i = 0; i != size; ++i) {   // i == columns
            mesh[j][stride * i]     = (float) i;    // x coordinate
            mesh[j][stride * i + 1] = (float) noiseGenerator.noise(i, j, 0);    // y coordinate
            mesh[j][stride * i + 2] = (float) j;    // z coordinate
        }
    }
    // second, calculate and assign normals
    // we have up to 6 points adjacent to the one being processed
    // use them to calculate a normal vector by going through them in
    // counter-clockwise order
    for (int j = 0; j != size; ++j) {
        for (int i = 0; i != size; ++i) {
            glm::vec3 normal = calculateNormals(i, j, size, mesh);
            mesh[j][stride * i + normalOffset]     = normal.x;
            mesh[j][stride * i + normalOffset + 1] = normal.y;
            mesh[j][stride * i + normalOffset + 2] = normal.z;
        }
    }
    // lastly, fill in UV coordinates
    float texU = 0.0f, texV = 0.0f;
    float step;
    if (repeat == 0)
        step = 1.0f / (float) size;
    else
        step = 1.0f / (float) repeat;
    for (int j = 0; j != size; ++j) {
        for (int i = 0; i != size; ++i) {
            mesh[j][stride * i + textureOffset    ] = texU;
            mesh[j][stride * i + textureOffset + 1] = texV;
            texU += step;
        }
        texU = 0.0f;
        texV += step;
    }
    return mesh;
}

void freeTerrain(float **& mesh, int size) {
    for (int i = 0; i != size; ++i)
        delete[] mesh[i];
    delete[] mesh;
    mesh = nullptr;
}