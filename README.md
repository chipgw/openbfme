OpenBFME
=========
Currently in very early prototype/groundwork stages, OpenBFME seeks to be an open source RTS engine, capable of playing games made with EA's SAGE engine, such as the Battle for Middle Earth series and some of the Command and Conquer games. (First target is to get it working with BFME 2) This will allow for playing said games on Linux (I'd say OSX as well, but I don't have one to develop on) and online without LAN emulation tools.
It is currently *nowhere* near playable, with only a small amount of groundwork done on a filesystem abstraction layer for .BIG archives, a parser for INI files, and the core game logic. I'm putting it on GitHub early mostly because I tend to neglect it otherwise, but I'm open to getting help from anyone interested as well.

Dependencies:
-------------
* [CMake]
* [GLM]

On Linux:
* [Boost] filesystem library.
* Compiler with C++11 support.

On Windows:
* [Visual Studio] 2015.
* OR [MinGW] 4.8 or greater & [Boost] filesystem.

Building
--------
* Clone or download the source code.
* In the source folder, create a `build` folder.
* In the build folder, run `cmake ..`. (TODO - Add info about options.)
* If you want to use a different generator than your platform default, add `-G <generator>` to the cmake command, with your desired generator. A list of generators can be found by running `cmake -h`.
* The project files should now be generated in `build`.

License
-------
[MIT License]


[CMake]:http://www.cmake.org
[GLM]:http://glm.g-truc.net/
[Boost]:http://boost.org
[Visual Studio]:http://www.visualstudio.com/
[MinGW]:http://www.mingw.org/
[MIT License]:LICENSE
