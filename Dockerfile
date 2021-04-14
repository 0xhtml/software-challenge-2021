FROM ubuntu:20.04

RUN apt update && env DEBIAN_FRONTEND=noninteractive apt install -y --no-install-recommends cmake build-essential libboost-all-dev zip

VOLUME /data
WORKDIR /data
CMD /bin/bash -c "./build.sh main release docker && cd cmake-docker && zip -9j build.zip main"
