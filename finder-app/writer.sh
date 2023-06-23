#!/bin/bash

if [ $# -lt 2 ]
then
	echo "Need two arguments: #1 <FILENAME> and #2 <STRING_TO_WRITE> to write)\, but received only $#."
	exit 1
fi

WRITE_FILE=$1
WRITE_STR=$2
BASE_DIR="$(dirname $WRITE_FILE)"

if [ ! -d $BASE_DIR ]
then
	mkdir -p $BASE_DIR
fi

echo "$WRITE_STR" > "$WRITE_FILE" || echo Write failed with exit code $?
