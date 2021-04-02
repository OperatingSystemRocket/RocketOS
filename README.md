# <p align="center"> RocketOS </p>

[![C/C++ CI](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml/badge.svg)](https://github.com/OperatingSystemRocket/RocketOS/actions/workflows/ci-cd.yml)
[![Docs](https://readthedocs.org/projects/rocketos/badge/?version=latest)](https://rocketos.readthedocs.io/en/latest/?badge=latest)
[![Pre-Releases](https://img.shields.io/github/release-date-pre/OperatingSystemRocket/RocketOS)](https://github.com/OperatingSystemRocket/RocketOS/releases)


<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#development-environment">Development Environment</a>
    </li>
    <li>
      <a href="#how-to-install-rocketos-without-the-need-for-developing-editing-the-code">Installing/Getting the iso for RocketOS</a>
    </li>
    <li>
      <a href="#how-want-to-contribute">How to Contribute</a>
    </li>
    <li>
      <a href="#where-can-i-find-the-docs">Link the the documentation</a>
    </li>
  </ol>
</details>



## Development Environment

This guide assumes a unix environment with access to the `apt` package manager.
You will have to manually edit the install script on other platforms.
This README.md used to hold an a list of all the needed software to develop this OS and how to install them,
but instead we have decided to provide an installation bash script. To use it, just download `toolchain.sh` and run it.
Follow any prompts for putting in your root password. This script will be building gcc from source, so it might take a while to finish. 


## How to install RocketOS (without the need for developing/editing the code)

This is very simple, just click on the release icon in this REAME.md, clicking on the github releases tab, or going to:
<a href="https://github.com/OperatingSystemRocket/RocketOS/releases">releases</a>
Here you will see a copy of the `LICENSE` (MIT) and a copy of the iso.
You can download and use this iso without any software. Just flash it to a usb or run it in a VM and you're good to go.


## How want to contribute

RocketOS is currently very early in its development, but we do have a CI/CD system and can handle external pull requests.
The most useful thing currently would be contributions or filing issues (though issues of any kind are welcome)
in unit tests and documentation as there is currently a backlog
and because this something that takes a lot of time and is currently a WIP.
To contribute you must agree to have your code and changes licensed under MIT.
We do not currently require you to forfeit your individual copyright to the code.


## Where can I find the docs?

<a href="https://rocketos.readthedocs.io/en/latest/">documentation</a>
