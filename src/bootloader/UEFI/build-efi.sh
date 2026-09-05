#!/usr/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")

clang \
    -I/usr/include \
    -target x86_64-unknown-uefi \
    -nostdlib \
    -Wl,-entry:efi_main \
    -o "$SCRIPT_DIR"/bootx64.efi \
    "$SCRIPT_DIR"/test.c
