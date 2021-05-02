#include "Scene.hpp"

namespace method {

Prop::Prop(HotloaderIndex mesh, const Material & material)
    : mesh_handle(mesh)
    , material(&material) {}

Scene::Scene()
    : point_lights_length(0) {}

void Scene::add_point_light(const PointLight & light) {
    if (point_lights_length < METHOD_MAX_POINT_LIGHTS) {
        point_lights_positions[point_lights_length] = light.position;
        point_lights_colors[point_lights_length] = light.color;
        point_lights_length++;
    }
}

void Scene::set_sun(const DirectionLight & light) {
    sun.direction = light.direction;
    sun.color = light.color;
}

}
