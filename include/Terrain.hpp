#pragma once

#include "MeshData.hpp"

#define METHOD_TERRAIN_POWER_LIMIT 8

namespace method {

/**
 * \brief Generates a rectangular triangle mesh.
 *
 * \param power The number of edge subdivisions of the rectangle.
 * \param width The width of the rectangular mesh.
 * \param height The height of the rectangular mesh.
 */
MeshData * new_terrain(int power, float width, float height);

}
