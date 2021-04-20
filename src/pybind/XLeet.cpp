//
// Created by per on 28.01.18.
//
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_constants(py::module &);

void init_gameshape(py::module &);
void init_gameengine(py::module &);
void init_enemy(py::module &);
void init_bulletfactory(py::module &);
void init_bullet(py::module &);
void init_background(py::module &);
void init_object(py::module &);
void init_player(py::module &);
void init_powerup(py::module &);
void init_resourcehandler(py::module &);
void init_scene(py::module &);
void init_script(py::module &);
void init_shooter(py::module &);
void init_vectorn(py::module &);
void init_world(py::module &);
void init_menu(py::module &);
void init_renderer(py::module &);
void init_image(py::module &);

void init_version(py::module &m) {
#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

#define INITS \
init_constants(m); \
init_gameshape(m); \
init_gameengine(m); \
init_enemy(m); \
init_bulletfactory(m); \
init_bullet(m); \
init_background(m); \
init_object(m); \
init_player(m); \
init_powerup(m); \
init_resourcehandler(m); \
init_scene(m); \
init_script(m); \
init_shooter(m); \
init_vectorn(m); \
init_world(m);\
init_menu(m);\
init_renderer(m);\
init_image(m);




#ifndef EMBEDDED
PYBIND11_MODULE(xleet, m) {
    m.doc() = "XLeet Engine";
    INITS
}
#else
PYBIND11_EMBEDDED_MODULE(xleet, m){
    m.doc() = "XLeet Engine";
    INITS
}
#endif