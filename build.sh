#!/bin/bash

OUTPUT_DIR=bin/
BUILD_DIR=build/
EXECUTABLE_NAME=anzu

function build () {
	BUILD_MODE="debug"
	if [ "$1" == '-r' ]; then
		BUILD_MODE="release";
	fi

	meson "$BUILD_DIR" -Dbuildtype="$BUILD_MODE";
	cd "$BUILD_DIR";
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
	echo "-r      Force build in release mode"
}

case "$1" in
	"build") build $2 ;;
	"run") run ;;
	"br") build $2 && run ;;
	"clean") clean ;;
	"help") show_help ;;
	*) show_help ;;
esac
