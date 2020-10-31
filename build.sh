#!/bin/bash

OUTPUT_DIR=bin/
BUILD_DIR=builddir/
EXECUTABLE_NAME=anzu

function build {
	meson "$BUILD_DIR" && cd $_;
	meson compile;
	cd .. && mkdir -p "$OUTPUT_DIR";
	mv "$BUILD_DIR$EXECUTABLE_NAME" "$OUTPUT_DIR";
}

function run { cd "$OUTPUT_DIR" && ./"$EXECUTABLE_NAME"; }

function clean { rm -r "$BUILD_DIR" && rm "$OUTPUT_DIR$EXECUTABLE_NAME"; }

function show_help {
	echo "Invalid argument!";
	echo "build   Build source code";
	echo "run     Run compiled binary";
	echo "br      Build and run"
	echo "clean   Delete build files"
	echo "help    Show this message"
}

case "$1" in
	"build") build ;;
	"run") run ;;
	"br") build && run ;;
	"clean") clean ;;
	"help") show_help ;;
	*) show_help ;;
esac
