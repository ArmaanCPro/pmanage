# pmanage

Cross-Platform process management in modern C++.

## Building

Builds like any other cmake project, with sane default options.
Use the given presets if you'd like.

Builds as a shared library. Symbols are hidden by default on all platforms.

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

The API is bound to change, but for the up-to-date API, look at `src/process.cppm`.

## Dependencies

The only dependency is `googletest` if you're building the tests. Otherwise, no dependencies are downloaded.

Make sure you have CMake >= 3.30 and a C++23 compiler if you want to build the project.
As for generators, you need either Ninja or Visual Studio. Any other generators are unreliable with Modules.

## CMake Configure Flags | Options

Example usage: `cmake --preset debug -DPMANAGE_BUILD_TESTS=OFF`.
You can also set these environment variables in your own project if you have this added as a dependency.

| Flag                |   | Description                                | Default | Note                  |
|---------------------|:--|--------------------------------------------|---------|-----------------------|
| PMANAGE_BUILD_TESTS |   | Build unit tests.                          | ON      |                       |
| ENABLE_ASAN         |   | Enable AddressSanitizer                    | OFF     |                       |
| ENABLE_UBSAN        |   | Enable UndefinedBehaviorSanitizer          | OFF     | Does nothing on MSVC. |
| ENABLE_TSAN         |   | Enable ThreadSanitizer                     | OFF     | Does nothing on MSVC. |
| ENABLE_IPO          |   | Enable interprocedural optimization (LTO). | OFF     |                       |

