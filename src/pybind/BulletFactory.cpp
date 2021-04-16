//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/BulletFactory.h"

namespace py = pybind11;

void init_bulletfactory(py::module &m) {
    py::class_<BulletFactory>(m, "BulletFactory");
}