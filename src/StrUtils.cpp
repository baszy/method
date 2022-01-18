#include "StrUtils.hpp"

#include <cstring>

using namespace std;

namespace method {

char * strndup(const char * source, size_t length) {
    char * result = new char[length + 1];
    strncpy(result, source, length);
    result[length] = '\0';
    return result;
}

}
