//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/ResourceManager.h"

namespace py = pybind11;

void init_resourcehandler(py::module &m) {
    py::class_<ResourceManager>(m, "ResourceManager");
}