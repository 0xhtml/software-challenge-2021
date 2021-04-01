FROM ubuntu:20.04

RUN apt update && env DEBIAN_FRONTEND=noninteractive apt install -y --no-install-recommends cmake build-essential libboost-all-dev zip

VOLUME /data
WORKDIR /data/cmake-build-docker
CMD /bin/bash -c "cmake .. && cmake --build . --config Release --target main && zip -9j build.zip main"
