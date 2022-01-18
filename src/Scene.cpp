#include "Scene.hpp"

using namespace std;

namespace method {

Prop::Prop(string mesh, const Material & material)
    : mesh(mesh),
      material(material) {}

Scene::Scene(const Camera & c, const Sky & s)
    : camera(&c),
      sky(&s),
      point_lights_length(0) {}

void Scene::add_point_light(const PointLight & light) {
    if (point_lights_length < METHOD_MAX_POINT_LIGHTS) {
        point_lights_positions[point_lights_length] = light.position;
        point_lights_colors[point_lights_length] = light.color;
        point_lights_length++;
    }
}

Vec3 * Scene::get_aabb() const {
    Vec3 * result = new Vec3[8];

    return result;
}

}
