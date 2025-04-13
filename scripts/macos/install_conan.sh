if [ -n "$BASH_VERSION" ]; then
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
elif [ -n "$ZSH_VERSION" ]; then
  SCRIPT_DIR="$(cd "$(dirname "${(%):-%x}")" && pwd)"
fi
ROOT_DIR="${SCRIPT_DIR}/../.."
VENV_DIR="${ROOT_DIR}/.venv"

python3 -m venv $VENV_DIR && \
$VENV_DIR/bin/python3 -m pip install -r $ROOT_DIR/requirements.txt