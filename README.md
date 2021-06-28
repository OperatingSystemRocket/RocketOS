<p align="center">
<img src="https://github.com/OperatingSystemRocket/RocketOS/blob/master/logo/Rocket.svg" alt="RocketOS logo"></img>
</p>

# <p align="center"> RocketOS </p>

<p align="center">
<a href="https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml">
  <img src="https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml/badge.svg" alt="C/C++ CI"></img>
</a>
<a href="https://rocketos.readthedocs.io/en/latest/?badge=latest">
  <img src="https://readthedocs.org/projects/rocketos/badge/?version=latest" alt="Docs"></img>
</a>
<a href="https://github.com/OperatingSystemRocket/RocketOS/releases">
  <img src="https://img.shields.io/github/release-date-pre/OperatingSystemRocket/RocketOS" alt="Pre-Releases"></img>
</a>
<a href="https://mit-license.org/">
  <img src="https://shields.io/badge/license-MIT-green" alt="License"></img>
</a>
</p>


## What is RocketOS?

RocketOS is a Unix based OS that uses legacy BIOS and GRUB and is written in C17.
It is being developed for educational purposes primarily, but it still is a serious project.
It is currently in its infancy.


## Development Environment

This guide assumes a unix environment with access to the `apt` package manager.
You will have to manually edit the install script on other platforms.
This repo provides a bash script that when run will install and setup all software required to develop RocketOS.
To use it, just download `toolchain.sh` and run it.
Follow any prompts for putting in your root password.
This script will be building gcc from source, so it might take a while to finish. 


## How to install RocketOS

Navigate to <a href="https://github.com/OperatingSystemRocket/RocketOS/releases">releases</a>.

You should also have obtained a copy of the MIT license together with this project.
If not, go to: https://mit-license.org/.
You can download and use this iso without any software.
Flash the iso to a USB or run it in a VM and then the OS should boot and dump you into a terminal.


## How to contribute

RocketOS is currently very early in its development, but we do have a CI/CD system and can handle external pull requests.
The most useful thing currently would be contributions or filing issues (though issues of any kind are welcome)
in unit tests and documentation as there is currently a backlog.
To contribute you must agree to have your code and other changes licensed under the MIT license.
We do not currently require you to forfeit your individual copyright to your changes.


## Where can I find the docs?

<a href="https://rocketos.readthedocs.io/en/latest/">Documentation</a>


## Roadmap

- [x] Global Descriptor Table
- [x] CPU Interrupts
- [x] VGA Driver
- [x] Keyboard Support
- [x] Basic Working Text Terminal
- [x] Serial Driver
- [ ] Mouse Support
- [x] Paging and Virtual Memory
- [x] Dynamic Memory Allocation
- [x] (PIC) Timer
- [-] Tasks, Processes, and Scheduling (In Progress)
- [-] Floating Point Number Support (currently has a partial implementation)
- [ ] Time and Calendar
- [x] Privilege Levels (Protection Rings)
- [ ] ELF Loader (currently has a partial implementation)
- [x] Disk/HDD Support (ATA driver)
- [ ] Filesystem
- [ ] Full libc Implementation
- [ ] Basic GUI Support
