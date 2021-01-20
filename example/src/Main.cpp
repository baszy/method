#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "Angle.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "GLShader.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "MeshData.hpp"
#include "OBJLoader.hpp"
#include "ReadFile.hpp"
#include "RenderTarget.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "Terrain.hpp"
#include "Vector.hpp"

#include "GLTexture.hpp"
#include "TGAImage.hpp"
#include "Controller.hpp"

using namespace method;

float clamp(float input, float min, float max) {
    if (input < min) return min;
    else if (input > max) return max;
    return input;
}

int main() {
    // TODO: (global) path or filename?

    RenderTarget window("method_example", IVec2(1280, 720));
    window.set_swap_mode(SwapMode::VSYNC);
    RenderSystem renderer = RenderSystem(&window);

    Controller input = Controller();
    input.grab_cursor(false);

    GLShader shader = GLShader();
    shader.load("example/shader/phong.vert", ShaderType::VERTEX);
    shader.load("example/shader/phong.frag", ShaderType::FRAGMENT);
    shader.compile();

    IVec2 size = window.get_dimensions();
    Camera camera = Camera(radians(70.0f), (float)size.x / (float)size.y,
        0.1f, 500.0f);

    Light light = Light();
    light.position = Vec3(5.0f, 5.0f, 5.0f);
    light.color = Vec3(1.0f, 1.0f, 1.0f);

    TGAImage * crate_diffuse_image = new TGAImage("example/img/wood_floor/diffuse.tga");
    GLTexture crate_diffuse = GLTexture(crate_diffuse_image);
    TGAImage * crate_normal_image = new TGAImage("example/img/wood_floor/normal.tga");
    GLTexture crate_normal = GLTexture(crate_normal_image);
    TGAImage * crate_specular_image = new TGAImage("example/img/wood_floor/specular.tga");
    GLTexture crate_specular = GLTexture(crate_specular_image);

    Material * crate = new Material();
    crate->diffuse_color = Vec3(0.329, 0.235, 0.145);
    crate->specular_exponent = 256;
    crate->diffuse_map = &crate_diffuse;
    crate->normal_map = &crate_normal;
    crate->specular_map = &crate_specular;
    crate->shader = &shader;

    MeshData * mesh = load_obj("example/resource/stanford_dragon.obj");
    Prop object = Prop(mesh, crate);
    object.set_position(Vec3(0.0f, 1.0f, 0.0f));
    object.set_scale(Vec3(10.0f, 10.0f, 10.0f));
    
    TGAImage * bog_diffuse_image = new TGAImage("example/img/ground/diffuse.tga");
    GLTexture bog_diffuse = GLTexture(bog_diffuse_image);
    TGAImage * bog_normal_image = new TGAImage("example/img/ground/normal.tga");
    GLTexture bog_normal = GLTexture(bog_normal_image);
    TGAImage * bog_specular_image = new TGAImage("example/img/ground/specular.tga");
    GLTexture bog_specular = GLTexture(bog_specular_image);

    Material * bog = new Material();
    bog->diffuse_color = Vec3(0.969, 0.647, 0.463);
    bog->specular_exponent = 32;
    bog->diffuse_map = &bog_diffuse;
    bog->normal_map = &bog_normal;
    bog->specular_map = &bog_specular;
    bog->shader = &shader;

    MeshData * triangles = new_terrain(4, 200.0f, 200.0f);
    Prop ground = Prop(triangles, bog);
    ground.set_position(Vec3(-100.0f, 0.0f, -100.0f));

    Scene scene = Scene();
    scene.cameras.push_back(&camera);
    scene.lights.push_back(&light);
    scene.props.push_back(&object);
    scene.props.push_back(&ground);

    Vec3 pos, vel;
    Vec2 angles;
    float speed = 0.05f;
    mat4 transform;

    int frame = 0;
    while (input.running) {
        vel = Vec3(0.0f, 0.0f, 0.0f);

        input.update();

        angles = angles + Vec2(radians(input.mouse_axis.x * 0.05f),
                               radians(-input.mouse_axis.y * 0.05f));
        angles.y = clamp(angles.y, -M_PI_2 + 0.01, M_PI_2 - 0.01);

        Vec3 front(cos(angles.x) * cos(angles.y),
                   sin(angles.y),
                   sin(angles.x) * cos(angles.y));
        Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 side = normalize(cross(front, up));

        if (input.direction_axis.x > 0) {
            pos = pos + speed * side;
        } else if (input.direction_axis.x < 0) {
            pos = pos - speed * side;
        }
        if (input.direction_axis.y > 0) {
            pos.y = pos.y + speed;
        } else if (input.direction_axis.y < 0) {
            pos.y = pos.y - speed;
        }
        if (input.direction_axis.z > 0) {
            pos = pos - speed * front;
        } else if (input.direction_axis.z < 0) {
            pos = pos + speed * front;
        }

        camera.set_position(pos);
        camera.set_look(pos + front);

        renderer.begin();
        renderer.draw(scene);
        renderer.end();

        frame = (frame + 1);
    }

    // TODO: :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :( :(
    delete mesh;
    delete triangles;

    return 0;
}
