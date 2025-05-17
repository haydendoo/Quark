FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    llvm-dev \
    zlib1g-dev \
    libzstd-dev \
    libcurl4-openssl-dev \
    libedit-dev \
    clang-tidy

WORKDIR /app

COPY . .

RUN mkdir build && cd build \
    && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. \
    && make -j8

RUN ./lint.sh

RUN ./build/quark main.qrk -o main

CMD ["sleep", "infinity"]
