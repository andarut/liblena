SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="${SCRIPT_DIR}/../.."
VENV_DIR="${ROOT_DIR}/.venv"

python3 -m venv $VENV_DIR && \
$VENV_DIR/bin/python3 -m pip install -r $ROOT_DIR/requirements.txt && \
source $VENV_DIR/bin/activate