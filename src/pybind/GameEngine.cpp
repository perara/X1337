//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../../include/GameEngine.h"

namespace py = pybind11;

void init_gameengine(py::module &m) {
    py::class_<GameEngine>(m, "GameEngine")
            .def(py::init())
            .def("run_game", &GameEngine::runGame);
}


