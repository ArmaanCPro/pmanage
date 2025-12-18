# pmanage

Cross-Platform process management.

## Building

Builds like any other cmake project, with sane default options.
Use the given presets if you'd like.

GCC builds are unreliable. Maybe they work, maybe they don't.
Although GCC is usually my preferred compiler, C++ modules support is lacking on GCC.
Thus, the most preferred compiler is Clang, but MSVC is also tested. In fact, MSVC has traditionally had the best modules support.

i.e.:
- `cmake --preset debug -DCMAKE_CXX_COMPILER=clang++`
- `cmake --build --preset debug-test`
- `./bin/debug/pmanageTests`

## Installing

You can install this library like any other CMake package.
CMake FetchContent makes it trivial to add as a dependency to your project.

## Usage

Currently, only a C++ module is exposed (no header include).
This means you must be using C++ 20 or beyond, and that your build system must support modules (most modern systems do).
To import the module, just add `import pmanage;` into your code.

The API is bound to change, but for the up-to-date API, look at (src/process.cppm)[src/process.cppm].

