//
// Created by per on 4/20/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Image.h"

namespace py = pybind11;

void init_image(py::module &m) {
    pybind11::class_<Image>(m, "Image", pybind11::buffer_protocol())
            .def_buffer([](Image& im) -> pybind11::buffer_info {
                return pybind11::buffer_info(
                        // Pointer to buffer
                        im.data(),
                        // Size of one scalar
                        sizeof(unsigned char),
                        // Python struct-style format descriptor
                        pybind11::format_descriptor<unsigned char>::format(),
                        // Number of dimensions
                        3,
                        // Buffer dimensions
                        { im.rows(), im.cols(), im.channels() },
                        // Strides (in bytes) for each index
                        {
                                sizeof(unsigned char) * im.channels() * im.cols(),
                                sizeof(unsigned char) * im.channels(),
                                sizeof(unsigned char)
                        }
                );
            });
}