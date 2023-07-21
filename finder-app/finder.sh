#!/bin/sh

if [ $# -lt 2 ]
then
	echo "Need 2 arguments: <FILES_DIR> <SEARCH_STR>, but received only $#."
	exit 1
fi

FILES_DIR=$1
SEARCH_STR=$2

if [ ! -d $FILES_DIR ]
then
	echo "$1 is not a directory! Try again."
	exit 1
fi

X=$(find $FILES_DIR -type f | wc -l)
Y=$(grep -r $SEARCH_STR $FILES_DIR | wc -l)

echo "The number of files are $X and the number of matching lines are $Y"
