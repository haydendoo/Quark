FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    llvm-dev \
    zlib1g-dev \
    libzstd-dev \
    libcurl4-openssl-dev \
    libedit-dev

WORKDIR /app

COPY . .

RUN cmake . && make

CMD ["./Quark", "main.qrk", "-o", "main"]