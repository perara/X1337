//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Powerup.h"

namespace py = pybind11;

void init_powerup(py::module &m) {
    py::class_<Powerup>(m, "Powerup");
}