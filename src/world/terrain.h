#pragma once
//----------------------------------------------------------------------------------------
/**
 * \file       terrain.h
 * \author     Yury Udavichenka
 * \date       01/05/2022
 * \brief      noise and terrain generator
 *
 *  Header containing a Perlin noise generator class as well as a suite
 *  of functions designed to generate a terrain of arbitrary size
 *
*/
//----------------------------------------------------------------------------------------
#include <vector>

#include "config.h"
/// struct that is filled up during terrain generation and used
/// during terrain rendering
struct Terrain {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint diffuse;
    GLuint specular;
    float * mesh = nullptr;                 ///< raw data of vertices, normals and texture coordinates
    unsigned int * indices = nullptr;       ///< index data for EBOs and faster rendering
    unsigned int nrTriangles;               ///< indicator for how many triangles to render
    int size;                               ///< terrain side size (actual size is size * size)
};

/// Perlin noise generator
class Perlin {
public:
    /// Initializes with ref values for the permutation vector
    Perlin();
    /// Generates a new permutation vector based on seed value
    /// @param[in] seed for new permutation generation
    Perlin(unsigned int seed);
    /// Returns a noise value, Z can be left empty for 2D noise
    /**
     *
     * @param x             X input value
     * @param y             Y input value
     * @param z             Z input value
     * @return              returns noise value for specific XYZ coords
     */
    double noise(double x, double y, double z);
private:
    double fade(double t);

    double lerp(double t, double a, double b);

    double grad(int hash, double x, double y, double z);
private:
    /// permutation vector
    std::vector<int> mPermutation;
};
/// returns a 2D array with generated terrain
/// @param[in] size     terrain side size
/// @param[in] repeat   how often to wrap the texture
float ** generateTerrain2DMesh(int size, int repeat);
/// calculates the number of triangles in a terrain mesh
/// based on how big its side size is
/// @param[in] size     terrain side size
int calculateNrTriangles(int size);
/// generates indices based on terrain size and # of triangles
/// @param[in] size         terrain side size
/// @param[in] nrTriangles  how many triangles there are
unsigned int * generateTerrainIndices(int size, unsigned int nrTriangles);

void freeTerrain2DMesh(float **& mesh, int size);
/// returns workable terrain object with flattened terrain mesh
/// @param[in] size         terrain side size
/// @param[in] texRepeat    how often to wrap the texture
Terrain createTerrain(int size, int texRepeat);

void deleteTerrain(Terrain & terrain);