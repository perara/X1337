//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Bullet.h"

namespace py = pybind11;

void init_bullet(py::module &m) {
    py::class_<Bullet>(m, "Bullet");
}