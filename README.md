# liblena

## Requirements

Currently library building with `conan==2.15.0` inside **virtualenv**, the only requirements are `Python (>= 3.6)`.

Run script for your system to **first time** building `<BUILD_TYPE>` version of library.

**IMPORTANT! Run with `source` command, so virtualenv can be kept opened.**
```
source ./scripts/prepare_<OS>_<ARCH>_<BUILD_TYPE>.sh
```

Example:
```
source ./scripts/prepare_macos_x64_debug.sh
```

This script will install dependencies and link installed `cmake` into `.venv/bin/cmake`.

## Configure
FIRST SEE [Requirements](Requirements) !

From now on you can build with **cmake configs**. `conan-debug` or `conan-release` depending on your build type.

```
cmake --preset 'conan-debug'
```
or
```
cmake --preset 'conan-release'
```

## Build
```
ninja -j$(nproc) -C build/Debug
```
or
```
ninja -j$(nproc) -C build/Release
```

On macos you can use `sysctl -n hw.ncpu` instead of `nproc`. `nproc` can be installed with `brew install coreutils`. 
```
ninja -j$(sysctl -n hw.ncpu) -C build/Debug
```
or
```
ninja -j$(sysctl -n hw.ncpu) -C build/Release
```

## Run tests
Only in **debug** build:
```
ninja test -j$(nproc) -C build/Debug
```

## Goal for v1.0
Working JPEG (encoder and decoder)

## Releases
