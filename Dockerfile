# Build environment for turok2-decomp.
# Apple Silicon hosts: run with `--platform=linux/amd64` so Rosetta-for-Linux
# handles the x86_64 / i386 binaries (cc1 ELF, asn64.exe via wine).
#
# Build:    docker build --platform=linux/amd64 -t turok2-build .
# Run:      docker run --platform=linux/amd64 --rm -it -v "$PWD":/work turok2-build

FROM --platform=linux/amd64 debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y --no-install-recommends \
        binutils-mips-linux-gnu \
        build-essential \
        ca-certificates \
        cpp \
        dos2unix \
        git \
        libc6:i386 \
        libstdc++6:i386 \
        make \
        python3 \
        python3-pip \
        python3-venv \
        wine32 \
        wine \
        xxd && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /work

# Splat and friends are installed via pip at first build.
# Run `pip install -r tools/requirements.txt` inside the container after
# initial setup if needed.

CMD ["/bin/bash"]
