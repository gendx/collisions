#   Collisions - a real-time simulation program of colliding particles.
#   Copyright (C) 2011 - 2013  G. Endignoux
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.txt


TEMPLATE = app
INCLUDEPATH += . config/ edit/ graphic/ math/ simul/

# /!\ Update this for your platform /!\
INCLUDEPATH += /your/path/to/boost/

# Input
HEADERS += \
    config/config_cible.hpp \
    config/config_courbe.hpp \
    config/config_mutation.hpp \
    config/config_piston.hpp \
    config/config_population.hpp \
    config/config_profil.hpp \
    config/config_reaction.hpp \
    config/config_widgetcourbe.hpp \
    config/configuration.hpp \
    dispatcher.hpp \
    document.hpp \
    edit/color_delegate.hpp \
    edit/combo_delegate.hpp \
    edit/double_delegate.hpp \
    edit/edit_courbes.hpp \
    edit/edit_divers.hpp \
    edit/edit_obstacles.hpp \
    edit/edit_pistons.hpp \
    edit/edit_polygone.hpp \
    edit/edit_populations.hpp \
    edit/edit_transformations.hpp \
    edit/edit_withpolygone.hpp \
    edit/editeur.hpp \
    edit/spinbox_delegate.hpp \
    graphic/courbe.hpp \
    graphic/courbes_group.hpp \
    graphic/draw_polygone.hpp \
    graphic/graph.hpp \
    graphic/graph_zone.hpp \
    graphic/profil.hpp \
    graphic/widgetcourbe.hpp \
    graphic/widgetprofil.hpp \
    main_window.hpp \
    math/coord.hpp \
    math/coord.tpl \
    math/coord_io.tpl \
    math/coord_qio.tpl \
    math/polygone.hpp \
    math/segment.hpp \
    math/solveur.hpp \
    simul/boule.hpp \
    simul/collision.hpp \
    simul/event.hpp \
    simul/map_ligne.hpp \
    simul/mobile.hpp \
    simul/obstacle.hpp \
    simul/piston.hpp \
    simul/population.hpp \
    simul/simulateur.hpp \
    simul/time.hpp

SOURCES += \
    config/config_cible.cpp \
    config/config_courbe.cpp \
    config/config_mutation.cpp \
    config/config_piston.cpp \
    config/config_population.cpp \
    config/config_profil.cpp \
    config/config_reaction.cpp \
    config/config_widgetcourbe.cpp \
    config/configuration.cpp \
    dispatcher.cpp \
    document.cpp \
    edit/color_delegate.cpp \
    edit/combo_delegate.cpp \
    edit/double_delegate.cpp \
    edit/edit_courbes.cpp \
    edit/edit_divers.cpp \
    edit/edit_obstacles.cpp \
    edit/edit_pistons.cpp \
    edit/edit_polygone.cpp \
    edit/edit_populations.cpp \
    edit/edit_transformations.cpp \
    edit/edit_withpolygone.cpp \
    edit/editeur.cpp \
    edit/spinbox_delegate.cpp \
    graphic/courbe.cpp \
    graphic/courbes_group.cpp \
    graphic/draw_polygone.cpp \
    graphic/graph.cpp \
    graphic/graph_zone.cpp \
    graphic/profil.cpp \
    graphic/widgetcourbe.cpp \
    graphic/widgetprofil.cpp \
    main.cpp \
    main_window.cpp \
    math/polygone.cpp \
    math/segment.cpp \
    math/solveur.cpp \
    simul/boule.cpp \
    simul/collision.cpp \
    simul/event.cpp \
    simul/mobile.cpp \
    simul/piston.cpp \
    simul/population.cpp \
    simul/simulateur.cpp \
    simul/time.cpp

RESOURCES += \
    collisions.qrc

