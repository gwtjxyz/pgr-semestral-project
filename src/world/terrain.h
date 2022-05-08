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

float ** generateTerrain(int size, int repeat);

void freeTerrain(float **& mesh, int size);