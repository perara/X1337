//
// Created by per on 4/16/21.
//
#include <pybind11/pybind11.h>
#include "../../include/World.h"

namespace py = pybind11;

void init_world(py::module &m) {
    py::class_<World>(m, "World");
}
