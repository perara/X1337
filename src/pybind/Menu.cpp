//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Menu.h"

namespace py = pybind11;

void init_menu(py::module &m) {
    py::class_<Menu>(m, "Menu");
}