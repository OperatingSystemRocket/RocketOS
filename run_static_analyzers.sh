#!/bin/bash

clang-tidy kernel.c | tee warnings.txt
clang-format kernel.c --dry-run | tee warnings.txt -a
