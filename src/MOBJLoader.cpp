#include "MOBJLoader.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace method {

const ivec3 MOBJLoader::s_index_bias(1, 1, 1);

vec3 MOBJLoader::process_v(const std::string & line) {
    vec3 result;
    std::sscanf(line.c_str(), "v %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

vec2 MOBJLoader::process_vt(const std::string & line) {
    vec2 result;
    std::sscanf(line.c_str(), "vt %f %f", &result.x, &result.y);
    return result;
}

vec3 MOBJLoader::process_vn(const std::string & line) {
    vec3 result;
    std::sscanf(line.c_str(), "vn %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

void MOBJLoader::process_f(const std::string & line, ivec3 * face, ivec3 * uv,
    ivec3 * normal) {

    if (std::sscanf(line.c_str(), "f %i/%i/%i %i/%i/%i %i/%i/%i",
        &face->x, &uv->x, &normal->x,
        &face->y, &uv->y, &normal->y,
        &face->z, &uv->z, &normal->z) == 9) {

        *face = *face - s_index_bias;
        *uv = *uv - s_index_bias;
        *normal = *normal - s_index_bias;
    } else if (std::sscanf(line.c_str(), "f %i//%i %i//%i %i//%i",
        &face->x, &normal->x,
        &face->y, &normal->y,
        &face->z, &normal->z) == 6) {

        *face = *face - s_index_bias;
        *normal = *normal - s_index_bias;
    } else if (std::sscanf(line.c_str(), "f %i/%i %i/%i %i/%i",
        &face->x, &uv->x,
        &face->y, &uv->y,
        &face->z, &uv->z) == 6) {

        *face = *face - s_index_bias;
        *uv = *uv - s_index_bias;
    } else if (std::sscanf(line.c_str(), "f %i %i %i",
        &face->x,
        &face->y,
        &face->z) == 3) {

        *face = *face - s_index_bias;
    }
}

MMeshData * MOBJLoader::load(const std::string & filename) {
    std::ifstream file(filename);

    // TODO: instead of throwing exception, deal with error locally
    if (file.fail())
        throw std::runtime_error("File could not be opened");

    std::vector<vec3> v;
    std::vector<vec2> vt;
    std::vector<vec3> vn;

    std::vector<vec3> vertices;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;

    std::vector<ivec3> indices;

    // TODO: sections are gross
    std::string line;
    ivec3 face, uv, normal;
    char section = '\0';

    while (file.peek() != -1) {
        std::getline(file, line);

        switch (line[0]) {
        case '#':
            // # [...]
            break;

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
                normals.resize(v.size());
                section = 'f';
            }

            process_f(line, &face, &uv, &normal);

            if (vt.size() > 0) {
                uvs[face.x] = vt[uv.x];
                uvs[face.y] = vt[uv.y];
                uvs[face.z] = vt[uv.z];
            } else {
                uvs.push_back(vec2());
            }

            if (vn.size() > 0) {
                normals[face.x] = vn[normal.x];
                normals[face.y] = vn[normal.y];
                normals[face.z] = vn[normal.z];
            } else {
                normals.push_back(vec3());
            }

            indices.push_back(ivec3(face.x, face.y, face.z));
            break;

        default:
            // Unsupported format or intentionally ignored
            break;
        }
    }

    return new MMeshData(v, uvs, normals, indices);
}

}
