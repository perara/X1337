//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Script.h"

namespace py = pybind11;

void init_script(py::module &m) {
    py::class_<Script>(m, "Script");
}