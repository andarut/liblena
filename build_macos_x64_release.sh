# Install conan into venv
which python3 && \
python3 -m venv .venv && \
.venv/bin/python3 -m pip install -r requirements.txt && \
source .venv/bin/activate && \

# Build with conan
which conan && \
conan build . --build=missing --profile ./profiles/mac_x64_release