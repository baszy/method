#include "Camera.hpp"

namespace method {

// TODO: (Bug) Updating position does not update the view matrix!

const Vec3 Camera::up = Vec3(0.0f, 1.0f, 0.0f);

inline void Camera::regenerate_projection() {
    projection = perspective(fov, aspect, near, far);
}

inline void Camera::regenerate_view() {
    view = look_at(position, target, up);
}

Camera::Camera(float fov, float aspect, float near, float far, const Vec3 & target)
    : fov(fov),
      aspect(aspect),
      near(near),
      far(far),
      target(target) {

    regenerate_projection();
    regenerate_view();
}

void Camera::set_params(float fov, float aspect, float near, float far) {
    this->fov = fov;
    this->aspect = aspect;
    this->near = near;
    this->far = far;

    regenerate_projection();
}

void Camera::set_look(const Vec3 & target) {
    this->target = target;

    regenerate_view();
}

Vec4 * Camera::get_frustum() const {
    Vec4 * result = new Vec4[8];

    float htan = tan(aspect * (fov / 2));
    float vtan = tan((fov / 2));

    float near_w = near * htan;
    float far_w  = 10  * htan;
    float near_h = near * vtan;
    float far_h  = 10  * vtan;

    // TODO: Is look_at vector incorrect? negated
    result[0] = view * Vec4(-near_w,  near_h, near, 1.0f);
    result[1] = view * Vec4( near_w,  near_h, near, 1.0f);
    result[2] = view * Vec4(-near_w, -near_h, near, 1.0f);
    result[3] = view * Vec4( near_w, -near_h, near, 1.0f);
    result[4] = view * Vec4( -far_w,   far_h,  far, 1.0f);
    result[5] = view * Vec4(  far_w,   far_h,  far, 1.0f);
    result[6] = view * Vec4( -far_w,  -far_h,  far, 1.0f);
    result[7] = view * Vec4(  far_w,  -far_h,  far, 1.0f);

    return result;
}

}
