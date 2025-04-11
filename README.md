# liblena

## Requirements

Currently library building with `conan==2.15.0` inside **virtualenv**, the only requirements are `Python (>= 3.6)`.

Run script for your system to **first time** building `<BUILD_TYPE>` version of library.
```
./scripts/prepare_<OS>_<ARCH>_<BUILD_TYPE>.sh
```

Example:
```
./scripts/prepare_macos_x64_debug.sh
```

This script will install dependencies and link installed `cmake` into `.venv/bin/cmake`.

## Configure
FIRST SEE [Requirements](Requirements) !

From now on you can build with **cmake configs**. `conan-build` or `conan-release` depending on your build type.

```
cmake --preset 'conan-debug'
```
or
```
cmake --preset 'conan-release'
```

### Special cases
If you have `cmake<3.23` (and can't install version `>=3.23`):
```
cmake <PATH> -G Ninja -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=<BUILD_TYPE>
```

If you don't have `cmake` (and can't install it) you can find `cmake` executable path in `build/<BUILD_TYPE>/generators/CMakePresets.json` by key `cmakeExecutable`. You can "pin" it with your code editor/IDE and run **cmake present**.

## Build
```
ninja -j$(nproc) -C build/Debug
```
or
```
ninja -j$(nproc) -C build/Release
```

## Run tests
Only in **debug** build:
```
ninja test -j$(nproc) -C build/Debug
```

## Goal for v1.0
Working JPEG (encoder and decoder)

## Releases
