#pragma once

#include <vector>

#include "Camera.hpp"
#include "Entity.hpp"
#include "Material.hpp"
#include "MeshManager.hpp"

namespace method {

#define METHOD_MAX_POINT_LIGHTS 10

class Prop : public Entity {
public:
    const Material * material;
    HotloaderIndex mesh_handle;

    Prop(HotloaderIndex mesh, const Material & material);
};

struct DirectionLight {
    Vec3 direction;
    Vec3 color;
};

struct PointLight {
    Vec3 position;
    Vec3 color;
};

struct Scene {
    Camera * camera;
    // TODO: Should we use std::vector
    std::vector<Prop *> props;

    int point_lights_length;
    Vec3 point_lights_positions[METHOD_MAX_POINT_LIGHTS];
    Vec3 point_lights_colors[METHOD_MAX_POINT_LIGHTS];

    DirectionLight sun;

    Scene();

    void add_point_light(const PointLight & light);
    void set_sun(const DirectionLight & light);
};

}
