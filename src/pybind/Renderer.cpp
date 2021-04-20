//
// Created by per on 4/20/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Renderer.h"

namespace py = pybind11;

void init_renderer(py::module &m) {
    py::class_<Renderer>(m, "Renderer")
            .def("capture", &Renderer::capture);
}