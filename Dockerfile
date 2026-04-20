FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    cmake git g++ \
    libfmt-dev \
    libspdlog-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --config Release

ENTRYPOINT ["./build/agent-simulation"]