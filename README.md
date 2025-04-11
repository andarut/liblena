# liblena

## Build

```
./build_<OS>_<ARCH>_<BUILD_TYPE>.sh
```

## Run tests
Inside `liblena/build/Release` or `liblena/build/Debug` depending on your build.
```
ninja test
```

### Example (macos):
Thid will configure Python virtual environment, install conan there and build the library.
```
./build_macos_x64_debug.sh 
```

## Goal for v1.0
Working JPEG (encoder and decoder)

## Releases
