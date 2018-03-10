# Almost Legal Speed - platform running game
# Copyright (C) 2018  Alexander Kraus <nr4@z10.info>, Christoph Hess <christoph.s.hess@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)

project(almost-legal-speed)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" ${CMAKE_MODULE_PATH})

find_package(SFML COMPONENTS graphics window system REQUIRED)
message("SFML LIBS" ${SFML_LIBRARIES})

set(SPEED_SOURCES
    main.cpp
    # level_importer.cpp
    GameState.cpp
    GameLogic.cpp
    Renderer.cpp
    Player.cpp
)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(speed ${SPEED_SOURCES})
target_link_libraries(speed ${SFML_LIBRARIES})
