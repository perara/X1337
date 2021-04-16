//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Scene.h"

namespace py = pybind11;

void init_scene(py::module &m) {
    py::class_<Scene>(m, "Scene");
}