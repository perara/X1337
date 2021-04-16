//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/GameShape.h"

namespace py = pybind11;

void init_gameshape(py::module &m) {
    py::class_<GameShape>(m, "GameShape");
}