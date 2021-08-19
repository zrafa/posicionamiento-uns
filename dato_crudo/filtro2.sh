#!/bin/bash

cat $1 | while read l; do

	if (echo $l | grep -v Est | grep : > /dev/null); then
		MAC=$(echo $l | sed -e "s/ .*//g")
		LEVEL=$(echo $l | sed -e "s/.* //g")
		LEVEL=$((LEVEL-5))
		echo $MAC" "$LEVEL
	else
		echo $l | grep -v Est
	fi

done
