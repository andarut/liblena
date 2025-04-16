<div align="center">

<picture>
  <source media="(prefers-color-scheme: light)" srcset="/docs/liblena_logo_light.svg">
  <img alt="liblena logo" src="/docs/liblena_logo_dark.svg" width="50%" height="50%">
</picture>

for experiments with data compression techniques

<h3>
</h3>

[![macos](https://github.com/andarut/liblena/actions/workflows/macos.yml/badge.svg?branch=main&event=push)](https://github.com/andarut/liblena/actions/workflows/macos.yml)

</div>

---

## Features
TOOD: describe

## Requirements

Currently library building with `conan` inside **virtualenv**, the only requirements are `Python (>= 3.6)` and C++ compiler.

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
