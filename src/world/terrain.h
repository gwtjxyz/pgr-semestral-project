#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       terrain.h
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      A brief file description
 *
 *  A more detailed file description.
 *
*/
//----------------------------------------------------------------------------------------
#include <vector>

#include "config.h"

struct Terrain {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint diffuse;
    GLuint specular;
    float * mesh = nullptr;
    unsigned int * indices = nullptr;
    unsigned int nrTriangles;
    int size;
};

class Perlin {
public:
    // Initializes with ref values for the permutation vector
    Perlin();
    // Generates a new permutation vector based on seed value
    Perlin(unsigned int seed);
    // Returns a noise value, Z can be left empty for 2D noise
    double noise(double x, double y, double z);
private:
    double fade(double t);

    double lerp(double t, double a, double b);

    double grad(int hash, double x, double y, double z);
private:
    // permutation vector
    std::vector<int> mPermutation;
};

float ** generateTerrain2DMesh(int size, int repeat);

int calculateNrTriangles(int size);

unsigned int * generateTerrainIndices(int size, unsigned int nrTriangles);

void freeTerrain2DMesh(float **& mesh, int size);

Terrain createTerrain(int size, int texRepeat);

void deleteTerrain(Terrain & terrain);