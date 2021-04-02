# <p align="center"> RocketOS </p>

[![C/C++ CI](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml)
[![Docs](https://readthedocs.org/projects/rocketos/badge/?version=latest)](https://rocketos.readthedocs.io/en/latest/?badge=latest)
[![Releases](https://github.com/OperatingSystemRocket/RocketOS/releases/badge.svg)](https://github.com/OperatingSystemRocket/RocketOS/releases)


<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
  </ol>
</details>

## Getting Started

### Prerequisites

This is a list of the software you will need to use and develop this software.

* Be using a Unix system that supports bash


* A gcc compatible cross compiler capable of emitting flat elf binaries for i686 (do not use x86_64). 
  
    If you do not know how to do this, this is a good guide on how:
    
    https://wiki.osdev.org/GCC_Cross-Compiler


* An install of `xorriso`
  
  If on Ubuntu, you can type:
  
  `sudo apt install xorriso`


* You need a system install of grub for the utilities `grub-mkrescue` and if developing the software, you also need `grub-file`. If using a standard Ubuntu install, these should already be installed.

  If it isn't installed, type `sudo apt install grub-pc-bin`


* A copy of `build-essential` for utilities such as `make`.

  You can install this on Ubuntu with the following command:
  
  `sudo apt install build-essential`


* If planning on running the operating system in a VM, you must install `qemu` with the command:

  `sudo apt install qemu qemu-system-i386`


* For developers only: Install `clang-tidy` and `clang-format`.

    This can be done with:

    `sudo apt install clang clang-format clang-tidy`

### Installation

This guide assumes basic familiarity with a Unix Terminal and a Unix operating system (you may install WSL if on Windows).

#### Setup

1. Clone this repository with the following command:

    `git clone --recurse-submodules https://github.com/OperatingSystemRocket/RocketOS`
  
2. Enter the new directory

    `cd RocketOS/`

#### Running it in a VM

3. If running it in a VM (`qemu`), just type:

    `make run RELEASE=1`

#### Running it on Real Hardware

3. This part of the guide is incomplete and will be finished later.

#### Cleaning Up
4. Type `make clean`


### Development Installation

1. Follow the two steps in <a href="#Setup">Setup</a>

2. To only run the static analysis checks as well as the code format checks, type:

    `make run_static_analyzers`

3. To a debug build, type:

    `make build`

    To do a debug build and run it in `qemu`, type:
   
    `make run`

4. This part of the guide is incomplete and will be finished later.
