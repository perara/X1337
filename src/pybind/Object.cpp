//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Object.h"

namespace py = pybind11;

void init_object(py::module &m) {
    py::class_<Object>(m, "Object");
}