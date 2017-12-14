[![Build Status](https://travis-ci.org/aolgu003/BB-stack.svg?branch=master)](https://travis-ci.org/aolgu003/BB-stack)
[![codecov][codecov-badge]][codecov-link]


# BB-stack
Code stack for my beagle bone. First attempt at setting up at setting up a cross compiling environment between my Linux desktop machine and an embedded Linux platform. In addition my intention with repo is experiment with different design patterns for embedded development. I will be trying to keep my code clean and follow a tdd flow.

## Getting Started
This code base is meant to be cross compiled on a linux host for a ubuntu armhf OS running on a beagle bone. In order to start developing we need to install two things:
  - the cross compilation tool chain
  - An ARM emulator to run the cross compiled code

### Non-VM Setup
In order to expidite the development environment setup I have made a script that installs all the necessary (and some unnecessary) packages to get up and running. The script installs the following packages:
  - cmake
  - (unnecessary) qtcreator (with the ros pluggin for pretty syntax highlighting)
  - gcc-arm-linux-gnueabihf
  - g++-arm-linux-gnueabihf
  - libc6-dev-armhf-cross
  - qemu-user-static
  - qemu-user
  - lcov

Simply running:```./setup_env.sh``` In the terminal will automatically install the afformentioned packages.

### Docker Setup (RECOMMENED)
Run to build an image with the dev environment:
docker build -t bb-stack .

Run this to start to start a container with a BB-Stack directory mounted in it:
docker run -it -v /home/andrew/Projects/BB-stack/:/BB-Stack --rm --name=bb-dev bb-stack:latest bash

### Vagrant Setup (RECOMMENED)
If you would like to development from a non-Ubuntu machine or you simply don't want any of the packages listed above to polute your machine you can use Vagrant to automatically set up a VM for you. Assuming you have virtual box installed: https://www.virtualbox.org/wiki/Downloads

and Vagrant:
https://www.vagrantup.com/docs/installation/

The setup process is as easy as:
```vagrant up``` to build the VM and setup the environment inside the VM followed by ```vagrant ssh``` to access the machine from the /BB-stack directory.

## Building and running the code
To build the code run: ```run_build.sh``` and to run the unit tests run ```run_unitTests.sh``` from the /BB-stack directory. 

## Qt Creator IDE
### Launching and Opening Project
Run ```qtcreator``` and within the IDE open a project and select the CMakeLists.txt within /BB-stack as the project file 

### COMING EVENTUALLY: Remote Deployment from VM with qtcreator


[codecov-badge]:   https://codecov.io/gh/aolgu003/BB-stack/branch/master/graph/badge.svg
[codecov-link]:    https://codecov.io/gh/aolgu003/BB-stack
[codecov-image]:   https://codecov.io/gh/aolgu003/BB-stack/blob/master/img/Codecov.png
