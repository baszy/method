#include "Type/TVec2.hpp"
#include "Type/TVec3.hpp"
#include "Type/TVec4.hpp"

namespace method {

// TODO: implementation may be moved here

template class TVec2<bool>;
template class TVec2<int>;
template class TVec2<unsigned int>;
template class TVec2<float>;
template class TVec2<double>;

template class TVec3<bool>;
template class TVec3<int>;
template class TVec3<unsigned int>;
template class TVec3<float>;
template class TVec3<double>;

template class TVec4<bool>;
template class TVec4<int>;
template class TVec4<unsigned int>;
template class TVec4<float>;
template class TVec4<double>;

bool operator == (const TVec2<bool> & lhs, const TVec2<bool> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator == (const TVec2<int> & lhs, const TVec2<int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator == (const TVec2<unsigned int> & lhs, const TVec2<unsigned int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator == (const TVec3<bool> & lhs, const TVec3<bool> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator == (const TVec3<int> & lhs, const TVec3<int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator == (const TVec3<unsigned int> & lhs, const TVec3<unsigned int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator == (const TVec4<bool> & lhs, const TVec4<bool> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

bool operator == (const TVec4<int> & lhs, const TVec4<int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

bool operator == (const TVec4<unsigned int> & lhs, const TVec4<unsigned int> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

}
