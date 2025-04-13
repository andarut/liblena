SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="${SCRIPT_DIR}/.."
PROFILES_DIR="${ROOT_DIR}/profiles"

PROFILE_PATH="${PROFILES_DIR}/mac_x64_release"

# Install conan into venv
source ${SCRIPT_DIR}/macos/install_conan.sh

# Install with conan
conan install ${ROOT_DIR} --build=missing \
  --profile:host=$PROFILE_PATH \
  --profile:build=$PROFILE_PATH