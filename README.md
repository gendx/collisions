# Collisions

Collisions is a real-time simulation program of colliding particles. [![Build Status](https://travis-ci.org/gendx/collisions.svg?branch=master)](https://travis-ci.org/gendx/collisions)


## Installation

In order to build this software, you will need to install :
* Qt4, available at [qt-project](http://qt-project.org/downloads). Qt5 was not tested yet, but it should be compatible.
* A C++ compiler supporting the C++14 standard (for *shared_ptr* and *random* libraries, and *make_unique* helper function).

Building is tested with [Travis CI](https://travis-ci.org) on GCC 4.9 and Clang 3.5.


## Usage

You can create simulation projects which record initial conditions :
* populations of particles (position, mass, initial speed),
* shapes of obstacles,
* gravity,
* and so on...

There is an edit mode to modify these projects and a simulation mode to run them.


## Performance

An efficient algorithm has been designed to model collisions and is able to simulate 1,000 particles at a rate of 10,000 collisions per second (CPU 1.66 GHz).


## License

*Copyright (C) 2011 - 2015  G. Endignoux*

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see [www.gnu.org/licenses/gpl-3.0.txt](http://www.gnu.org/licenses/gpl-3.0.txt).

