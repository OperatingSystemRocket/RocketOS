# <p align="center"> RocketOS </p>

[![C/C++ CI](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml)
[![Docs](https://readthedocs.org/projects/rocketos/badge/?version=latest)](https://rocketos.readthedocs.io/en/latest/?badge=latest)
[![Pre-Releases](https://img.shields.io/github/release-date-pre/OperatingSystemRocket/RocketOS)](https://github.com/OperatingSystemRocket/RocketOS/releases)


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


## How to install RocketOS (without the need for developing/editing the code)

Navigate to <a href="https://github.com/OperatingSystemRocket/RocketOS/releases">releases</a>.

You should also have obtained a copy of the MIT license together with this project.
If not, go to: https://mit-license.org/.
You can download and use this iso without any software.
Flash the iso to a usb or run it in a VM and then the OS should boot and dump you into a terminal.


## How to contribute

RocketOS is currently very early in its development, but we do have a CI/CD system and can handle external pull requests.
The most useful thing currently would be contributions or filing issues (though issues of any kind are welcome)
in unit tests and documentation as there is currently a backlog.
To contribute you must agree to have your code and other changes licensed under the MIT license.
We do not currently require you to forfeit your individual copyright to your changes.


## Where can I find the docs?

<a href="https://rocketos.readthedocs.io/en/latest/">Documentation</a>
