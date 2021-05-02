#include "Terrain.hpp"

#include <cassert>
#include <cmath>
#include <vector>

#include "Vector.hpp"

namespace method {

MeshData * new_terrain(int power, float width, float height) {
    // This can be removed once large array support is fixed
	assert(power <= METHOD_TERRAIN_POWER_LIMIT);
			
	// The power dictates how many subdivisions this mesh will have. E.g. power
    // of 2 means 2^2 subdivisions = 4 sections squared, where each section is
    // 2 triangles.
    int size = pow(2, power);
    int npoints = size + 1;

    std::vector<Vec3> points;
    std::vector<Vec2> uvs;
    std::vector<Basis> bases;
    std::vector<IVec3> indices;

    for (int i = 0; i < npoints * npoints; i++) {
        // x = (i % npoints)
        // y = (i / npoints)

        points.push_back(Vec3((i % npoints) * (width / size), 0,
            (i / npoints) * (height / size)));
        //uvs.push_back(Vec2((float)(i % npoints) / size,
        //    (float)(i / npoints) / size));
        uvs.push_back(Vec2((float)(i % npoints), (float)(i / npoints)));
        bases.push_back({Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f)});
    }

    for (unsigned int i = 0; i < size * size + size - 1; i++) {
        if ((i + 1) % npoints == 0) i++;

        if (i & 1) { // i % 2 == 0
            indices.push_back(IVec3(i + 1, i, i + npoints));
            indices.push_back(IVec3(i + npoints, i + npoints + 1, i + 1));
        } else {
            indices.push_back(IVec3(i + npoints + 1, i + 1, i));
            indices.push_back(IVec3(i, i + npoints, i + npoints + 1));
	    }
    }

    MeshData * result = new MeshData(points.size(), indices.size());

    result->set_vertices(points.data());
    result->set_uvs(uvs.data());
    result->set_bases(bases.data());
    result->set_indices(indices.data());

    return result;
}

}
