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

<details>
<summary>About</summary>

RocketOS is a Unix based OS that uses legacy BIOS and GRUB and is written in C17.
It is being developed for educational purposes primarily, but it still is a serious project.
It is currently in its infancy.
</details>


## Development Environment

<details>
<summary>Setup</summary>

This guide assumes a unix environment with access to the `apt` package manager.
You will have to manually edit the install script on other platforms.
This repo provides a bash script that when run will install and setup all software required to develop RocketOS.
To use it, just download `toolchain.sh` and run it.
Follow any prompts for putting in your root password.
This script will be building gcc from source, so it might take a while to finish. 
</details>


## How to install RocketOS

<deatils>
<summary>Installation</summary>

Navigate to <a href="https://github.com/OperatingSystemRocket/RocketOS/releases">releases</a>.

You should also have obtained a copy of the MIT license together with this project.
If not, go to: https://mit-license.org/.
You can download and use this iso without any software.
Flash the iso to a USB or run it in a VM and then the OS should boot and dump you into a terminal.
</details>


## How to contribute

<details>
<summary>Information</summary>

RocketOS is currently very early in its development, but we do have a CI/CD system and can handle external pull requests.
The most useful thing currently would be contributions or filing issues (though issues of any kind are welcome)
in unit tests and documentation as there is currently a backlog.
To contribute you must agree to have your code and other changes licensed under the MIT license.
We do not currently require you to forfeit your individual copyright to your changes.
</details>


## Where can I find the docs?

<details>
<summary>Docs</summary>

<a href="https://rocketos.readthedocs.io/en/latest/">Documentation</a>
</details>
