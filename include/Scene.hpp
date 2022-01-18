#pragma once

#include <string>
#include <vector>

#include "Camera.hpp"
#include "Entity.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "MeshManager.hpp"
#include "Sky.hpp"

namespace method {

#define METHOD_MAX_POINT_LIGHTS 10

class Prop : public Entity {
public:
    const Material & material;
    std::string mesh;

    Prop(std::string mesh, const Material & material);
};

class Scene {
public:
    const Camera * camera;
    const Sky * sky;

    std::vector<Prop *> props;

    int point_lights_length;
    Vec3 point_lights_positions[METHOD_MAX_POINT_LIGHTS];
    Vec3 point_lights_colors[METHOD_MAX_POINT_LIGHTS];

    Scene(const Camera & c, const Sky & s);

    void add_point_light(const PointLight & light);

    Vec3 * get_aabb() const;
};

}
