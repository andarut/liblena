# liblena

## Requirements

Currently library building with `conan==2.15.0` inside **virtualenv**, the only requirements are `Python (>= 3.6)`.

`cmake` is optional dependency because `conan` with install it. But you will need to get path to `cmake` from conan CLI output. But basically any version of `cmake` will be working. Even version without supporting for presets.

Run script for your system to **first time** building `<BUILD_TYPE>` version of library.
```
./scripts/prepare_<OS>_<ARCH>_<BUILD_TYPE>.sh
```

Example:
```
./scripts/prepare_macos_x64_debug.sh
```

This script will install `cmake` and other dependencies.

## Build
FIRST SEE [Requirements](Requirements) !

From now on you can build with **cmake configs** (required `cmake>=3.23`). `conan-build` or `conan-release` depending on your build type.

```
cmake --preset 'conan-debug'
```
or
```
cmake --preset 'conan-release'
```

### Special build cases
If you have `cmake<3.23` (and can't install version `>=3.23`):
```
cmake <PATH> -G Ninja -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=<BUILD_TYPE>
```

If you don't have `cmake` (and can't install it) you can find this string in CLI output after running script from [Requirements](Requirements).
```
cmake/<CMAKE_VERSION>: Appending PATH environment variable: <CMAKE_BIN_PATH>
```
You can find `cmake` executable of version `<CMAKE_VERSION>` in `<CMAKE_BIN_PATH>` directory. You can "pin" it with your code editor/IDE and run **cmake present**.

## Run tests
Only in **debug** build:
```
ninja test -C build/Debug
```

## Goal for v1.0
Working JPEG (encoder and decoder)

## Releases
