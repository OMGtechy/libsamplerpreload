# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.231.6/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version (use hirsuite or bionic on local arm64/Apple Silicon): hirsute, focal, bionic
ARG VARIANT="hirsute"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update
RUN apt-get install -y --no-install-recommends software-properties-common=0.99.9.8
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update
RUN apt-get -y install --no-install-recommends make=4.2.1-1.2 cmake=3.16.3-1ubuntu1 gcc-10=10.3.0-1ubuntu1~20.04 g++-10=10.3.0-1ubuntu1~20.04 clang-11=1:11.0.0-2~ubuntu20.04.1 libunwind-dev=1.2.1-9build1
