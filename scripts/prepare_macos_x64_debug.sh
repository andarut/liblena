SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="${SCRIPT_DIR}/.."
PROFILES_DIR="${ROOT_DIR}/profiles"

PROFILE_PATH="${PROFILES_DIR}/mac_x64_debug"

# Create venv and install conan
source ${SCRIPT_DIR}/macos/install_conan.sh

# Activate venv
source ${ROOT_DIR}/.venv/bin/activate

# Install with conan
conan install ${ROOT_DIR} --build=missing \
  --profile:host=$PROFILE_PATH \
  --profile:build=$PROFILE_PATH

# Link cmake installed by conan
PRESETS_CONFIG_PATH="${ROOT_DIR}/build/Debug/generators/CMakePresets.json"
PYTHON_EXECUTABLE="${ROOT_DIR}/.venv/bin/python3"
CMAKE_PATH="$($PYTHON_EXECUTABLE -c 'import json, sys; presets = json.load(sys.stdin); print(presets["configurePresets"][0]["cmakeExecutable"])' < "$PRESETS_CONFIG_PATH")"
CMAKE_EXECUTABLE="${ROOT_DIR}/.venv/bin/cmake"

echo "Linking $CMAKE_PATH -> $CMAKE_EXECUTABLE"

rm -f $CMAKE_EXECUTABLE && \
ln -s $CMAKE_PATH $CMAKE_EXECUTABLE && \

# TODO: keep activated after script
source ${ROOT_DIR}/.venv/bin/activate