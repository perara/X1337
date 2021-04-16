//
// Created by per on 4/16/21.
//


#include <pybind11/pybind11.h>
#include "../../include/Background.h"

namespace py = pybind11;

void init_background(py::module &m) {
    py::class_<Background>(m, "Background");
}