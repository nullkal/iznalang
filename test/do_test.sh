#!/bin/sh

for file in ./tests/*.ina; do
	printf "testing '$(basename $file)'... "
	DIFF=$(../build/bin/izna < $file | diff ${file%.ina}.txt -)
	if [ "$DIFF" = "" ]; then
		echo "passed!"
	else
		echo "failed.."
		exit 1
	fi
done
