//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Shooter.h"

namespace py = pybind11;

void init_shooter(py::module &m) {
    py::class_<Shooter>(m, "Shooter");
}
