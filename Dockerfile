FROM alpine:latest AS build

RUN apk add --no-cache \
    build-base \
    cmake \
    g++

WORKDIR /app

COPY . .

RUN cmake . && make

FROM scratch

COPY --from=build /app/Quark /Quark
COPY main.qrk .

CMD ["./Quark", "main.qrk", "-o", "main"]