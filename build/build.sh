#!/usr/bin/env bash

APP_NAME="druid"

BUILD_DIR=$(pwd)
OUTPUT_DIR="$BUILD_DIR"/..
CODE_DIR="$OUTPUT_DIR"/code

gcc -I"$CODE_DIR" \
    -I"$CODE_DIR"/tests/ \
    -I"$CODE_DIR"/compiler_tools/ \
    -I"$CODE_DIR"/data_structures/ \
    "$CODE_DIR"/"$APP_NAME".c -Wall -Werror -o "$OUTPUT_DIR"/"$APP_NAME"
