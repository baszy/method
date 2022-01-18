#include "Material.hpp"

using namespace std;

namespace method {

Material::Material(string diffuse_handle,
                   string normal_handle,
                   string specular_handle)
    : diffuse_handle(diffuse_handle),
      normal_handle(normal_handle),
      specular_handle(specular_handle) {}

}
