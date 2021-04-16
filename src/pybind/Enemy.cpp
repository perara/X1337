//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Enemy.h"

namespace py = pybind11;

void init_enemy(py::module &m) {
    py::class_<Enemy>(m, "Enemy");
}