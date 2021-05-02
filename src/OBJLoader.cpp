#include "OBJLoader.hpp"

#include <fstream>
#include <memory>
#include <vector>

namespace method {

// see C++11 (draft N3337) §3.5/4
namespace {

Vec3 process_v(const std::string & line) {
    Vec3 result;
    std::sscanf(line.c_str(), "v %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

Vec2 process_vt(const std::string & line) {
    Vec2 result;
    std::sscanf(line.c_str(), "vt %f %f", &result.x, &result.y);
    return result;
}

Vec3 process_vn(const std::string & line) {
    Vec3 result;
    std::sscanf(line.c_str(), "vn %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

void process_f(const std::string & line, IVec3 * face, IVec3 * uv,
    IVec3 * normal) {

    if (std::sscanf(line.c_str(), "f %i/%i/%i %i/%i/%i %i/%i/%i",
        &face->x, &uv->x, &normal->x,
        &face->y, &uv->y, &normal->y,
        &face->z, &uv->z, &normal->z) == 9) {

        *face = *face - METHOD_OBJ_INDEX_BIAS;
        *uv = *uv - METHOD_OBJ_INDEX_BIAS;
        *normal = *normal - METHOD_OBJ_INDEX_BIAS;
    } else if (std::sscanf(line.c_str(), "f %i//%i %i//%i %i//%i",
        &face->x, &normal->x,
        &face->y, &normal->y,
        &face->z, &normal->z) == 6) {

        *face = *face - METHOD_OBJ_INDEX_BIAS;
        *normal = *normal - METHOD_OBJ_INDEX_BIAS;
    } else if (std::sscanf(line.c_str(), "f %i/%i %i/%i %i/%i",
        &face->x, &uv->x,
        &face->y, &uv->y,
        &face->z, &uv->z) == 6) {

        *face = *face - METHOD_OBJ_INDEX_BIAS;
        *uv = *uv - METHOD_OBJ_INDEX_BIAS;
    } else if (std::sscanf(line.c_str(), "f %i %i %i",
        &face->x,
        &face->y,
        &face->z) == 3) {

        *face = *face - METHOD_OBJ_INDEX_BIAS;
    }
}

}

MeshData * load_obj(const std::string & path) {
    std::ifstream file(path);

    // TODO: are we even going to deal with the error?
    if (file.fail()) return nullptr;

    // TODO: we shouldnt be using vec
    std::vector<Vec3> v;
    std::vector<Vec2> vt;
    std::vector<Vec3> vn;

    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Basis> bases;

    std::vector<IVec3> indices;

    // TODO: sections are gross
    std::string line;
    IVec3 face, uv, normal;
    char section = '\0';

    while (file.peek() != -1) {
        std::getline(file, line);

        switch (line[0]) {
        case 'v':
            // v x y z [w]
            // vt u [v] [w]
            // vn i j k
            section = 'v';
            if (line[1] == 't')
                vt.push_back(process_vt(line));
            else if (line[1] == 'n')
                vn.push_back(process_vn(line));
            else
                v.push_back(process_v(line));
            break;

        case 'p':
            // p v1 [...]
            section = 'p';
            break;

        case 'l':
            // l v1/vt1 v2/vt2 [...]
            section = 'l';
            break;

        case 'f':
            // f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3] [...]
            if (section != 'f') {
                vertices.resize(v.size());
                uvs.resize(v.size());
                bases.resize(v.size());
                section = 'f';
            }

            process_f(line, &face, &uv, &normal);

            if (vt.size() > 0) {
                uvs[face.x] = vt[uv.x];
                uvs[face.y] = vt[uv.y];
                uvs[face.z] = vt[uv.z];
            } else {
                uvs.push_back(Vec2());
            }

            if (vn.size() > 0) {
                bases[face.x].normal = vn[normal.x];
                bases[face.y].normal = vn[normal.y];
                bases[face.z].normal = vn[normal.z];
            } else {
                bases.push_back(Basis());
            }

            indices.push_back(IVec3(face.x, face.y, face.z));
            break;

        default:
            // Unsupported format or intentionally ignored
            break;
        }
    }

    MeshData * result = new MeshData(v.size(), indices.size());

    result->set_vertices(v.data());
    result->set_uvs(uvs.data());
    result->set_bases(bases.data());
    result->set_indices(indices.data());

    return result;
}

}
