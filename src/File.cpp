#include "File.hpp"

using namespace std;

namespace method {

File::File(string path)
    : path(path) {}

string File::get_path() const {
      return path;
}

}
