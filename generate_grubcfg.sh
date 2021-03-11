#!/bin/bash

touch $1
echo "set timeout=0" > $1
echo "set default=0" >> $1
echo "menuentry \"$2\"" { >> $1
echo "	multiboot $3" >> $1
echo "	boot" >> $1
echo "}" >> $1
