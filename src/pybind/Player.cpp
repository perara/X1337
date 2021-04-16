//
// Created by per on 4/16/21.
//

#include <pybind11/pybind11.h>
#include "../../include/Player.h"

namespace py = pybind11;

void init_player(py::module &m) {
    py::class_<Player>(m, "Player")
    .def("add_score", &Player::addScore)
    .def("get_player_score", &Player::getPlayerScore)
    .def("get_player_kills", &Player::getPlayerKills)
    .def("get_health", &Player::getHealth)
    .def("get_max_health", &Player::getStartHealth);
}