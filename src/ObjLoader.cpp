#include "ObjLoader.hpp"

#include "StrUtils.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

namespace std {

template<>
class hash<method::IVec3> {
public:
    size_t operator () (const method::IVec3 & key) const {
        size_t result = 17;
        result = result * 31 + hash<int>()(key.x);
        result = result * 31 + hash<int>()(key.y);
        result = result * 31 + hash<int>()(key.z);
        return result;
    }
};

}

using namespace std;

namespace method {

namespace {

Vec3 process_v(const char * line) {
    Vec3 result;
    sscanf(line, "v %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

Vec2 process_vt(const char * line) {
    Vec2 result;
    sscanf(line, "vt %f %f", &result.x, &result.y);
    return result;
}

Vec3 process_vn(const char * line) {
    Vec3 result;
    sscanf(line, "vn %f %f %f", &result.x, &result.y, &result.z);
    return result;
}

void process_f(const char * line, IVec3 * v1, IVec3 * v2, IVec3 * v3) {
    if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i",
        &v1->x, &v1->y, &v1->z,
        &v2->x, &v2->y, &v2->z,
        &v3->x, &v3->y, &v3->z) == 9) {}

    else if (sscanf(line, "f %i//%i %i//%i %i//%i",
        &v1->x, &v1->z,
        &v2->x, &v2->z,
        &v3->x, &v3->z) == 6) {}

    else if (sscanf(line, "f %i/%i %i/%i %i/%i",
        &v1->x, &v1->y,
        &v2->x, &v2->y,
        &v3->x, &v3->y) == 6) {}

    else if (sscanf(line, "f %i %i %i",
        &v1->x,
        &v2->x,
        &v3->x) == 3) {}

    *v1 = *v1 - METHOD_OBJ_INDEX_BIAS;
    *v2 = *v2 - METHOD_OBJ_INDEX_BIAS;
    *v3 = *v3 - METHOD_OBJ_INDEX_BIAS;
}

int process_s(const char * line) {
    int result;
    if (!sscanf(line, "s %d", &result)) result = 0;
    return result;
}

MeshData * load_obj_mo(const File & file) {
    vector<Vec3> v;
    vector<Vec2> vt;
    vector<Vec3> vn;

    class IGroup {
    public:
        vector<IVec3> vertices;
        vector<IVec3> faces;
    };

    int sgroup = 0;
    unordered_map<int, IGroup> igroups;

    const char * data = static_cast<const char *>(file.get_data());
    size_t data_length = file.get_size();

    size_t index = 0;
    while (index < data_length) {
        size_t start = index, end = index;
        while (data[end] != '\n' && end < data_length) end++;
        const char * line = strndup(data + start, end - start);
        index = end + 1;

        switch (line[0]) {
        case 'v': {
            // v x y z [w]
            if (line[1] == ' ') v.push_back(process_v(line));
            // vt u [v] [w]
            else if (line[1] == 't') vt.push_back(process_vt(line));
            // vn i j k
            else if (line[1] == 'n') vn.push_back(process_vn(line));
        } break;

        case 'f': {
            // f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3] [...]
            IVec3 v1, v2, v3;
            process_f(line, &v1, &v2, &v3);

            IVec3 face(-1);

            // TODO: literally O(n^2)
            IGroup & group = igroups[sgroup];
            for (size_t i = 0; i < group.vertices.size(); i++) {
                if (group.vertices[i] == v1)
                    face.x = i;
                if (group.vertices[i] == v2)
                    face.y = i;
                if (group.vertices[i] == v3)
                    face.z = i;
            }

            if (face.x == -1) {
                group.vertices.push_back(v1);
                face.x = group.vertices.size() - 1;
            }
            if (face.y == -1) {
                group.vertices.push_back(v2);
                face.y = group.vertices.size() - 1;
            }
            if (face.z == -1) {
                group.vertices.push_back(v3);
                face.z = group.vertices.size() - 1;
            }

            group.faces.push_back(face);
        } break;

        case 's': {
            // s [off] [group]
            sgroup = process_s(line);
            if (!igroups.count(sgroup)) igroups.emplace(sgroup, IGroup());
        } break;

        default: break;
        }

        delete [] line;
    }

    size_t base_index = 0;

    vector<Vec3> positions;
    vector<Vec2> uvs;
    vector<Basis> bases;
    vector<IVec3> indices;

    for (const auto & elem : igroups) {
        const IGroup & group = elem.second;
        for (const auto & elem : group.faces) {
            indices.push_back(elem + IVec3(base_index));
        }
        for (const auto & elem : group.vertices) {
            positions.push_back(v[elem.x]);
            uvs.push_back(vt[elem.y]);
            bases.emplace_back(vn[elem.z]);
        }
        base_index += group.vertices.size();
    }

    MeshData * result = new MeshData(base_index, indices.size());

    result->set_positions(positions.data());
    result->set_uvs(uvs.data());
    result->set_bases(bases.data());
    result->set_faces(indices.data());

    return result;
}

MeshData * load_obj_mo_hash(const File & file) {
    vector<Vec3> v;
    vector<Vec2> vt;
    vector<Vec3> vn;

    int group = 0;
    using SmoothGroup = unordered_map<IVec3, int>;
    unordered_map<int, SmoothGroup> groups;

    size_t vertices = 0;
    // TODO: Is reserve() possible?
    vector<Vec3> positions;
    vector<Vec2> uvs;
    vector<Basis> bases;
    vector<IVec3> indices;

    const char * data = static_cast<const char *>(file.get_data());
    size_t data_length = file.get_size();

    size_t index = 0;
    while (index < data_length) {
        size_t start = index, end = index;
        while (data[end] != '\n' && end < data_length) end++;
        const char * line = strndup(data + start, end - start);
        index = end + 1;

        switch (line[0]) {
        case 'v': {
            // v x y z [w]
            if (line[1] == ' ') v.push_back(process_v(line));
            // vt u [v] [w]
            else if (line[1] == 't') vt.push_back(process_vt(line));
            // vn i j k
            else if (line[1] == 'n') vn.push_back(process_vn(line));
        } break;

        case 'f': {
            // f v1[/vt1][/vn1] v2[/vt2][/vn2] v3[/vt3][/vn3] [...]
            IVec3 v1, v2, v3;
            process_f(line, &v1, &v2, &v3);

            IVec3 face;

            if (group == 0) {
                face.x = vertices++;
                positions.push_back(v[v1.x]);
                uvs.push_back(vt[v1.y]);
                bases.emplace_back(vn[v1.z]);

                face.y = vertices++;
                positions.push_back(v[v2.x]);
                uvs.push_back(vt[v2.y]);
                bases.emplace_back(vn[v2.z]);

                face.z = vertices++;
                positions.push_back(v[v3.x]);
                uvs.push_back(vt[v3.y]);
                bases.emplace_back(vn[v3.z]);
            } else {
                // Will construct a new group if one doesnt exist already
                SmoothGroup & faces = groups[group];

                if (faces.count(v1)) {
                    face.x = faces.at(v1);
                } else {
                    face.x = vertices++;
                    faces.emplace(v1, face.x);
                    positions.push_back(v[v1.x]);
                    uvs.push_back(vt[v1.y]);
                    bases.emplace_back(vn[v1.z]);
                }

                if (faces.count(v2)) {
                    face.y = faces.at(v2);
                } else {
                    face.y = vertices++;
                    faces.emplace(v2, face.y);
                    positions.push_back(v[v2.x]);
                    uvs.push_back(vt[v2.y]);
                    bases.emplace_back(vn[v2.z]);
                }

                if (faces.count(v3)) {
                    face.z = faces.at(v3);
                } else {
                    face.z = vertices++;
                    faces.emplace(v3, face.z);
                    positions.push_back(v[v3.x]);
                    uvs.push_back(vt[v3.y]);
                    bases.emplace_back(vn[v3.z]);
                }
            }

            indices.push_back(face);
        } break;

        case 's': {
            // s [off] [group]
            group = process_s(line);
        } break;

        default: break;
        }

        delete [] line;
    }

    MeshData * result = new MeshData(vertices, indices.size());

    // TODO: You a copier
    result->set_positions(positions.data());
    result->set_uvs(uvs.data());
    result->set_bases(bases.data());
    result->set_faces(indices.data());

    return result;
}

}

MeshData * load_obj(const File & file) {
    const chrono::time_point<std::chrono::system_clock> start = chrono::system_clock::now();
    MeshData * result = load_obj_mo_hash(file);
    const chrono::time_point<std::chrono::system_clock> end = chrono::system_clock::now();

    cout << "[ObjLoader][Notification/Other] Loaded " << file.get_path();
    cout << " in " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms,";
    cout << " converted to: " << endl;
    cout << "[ObjLoader][Notification/Other]   GL Vertices: " << result->num_vertices << endl;
    cout << "[ObjLoader][Notification/Other]   GL Faces: " << result->num_faces << endl;

    return result;
}

}
