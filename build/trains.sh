#!/bin/bash

array=()
function classes {
	array+=($(grep 'class.*{' $1 | cut -d ' ' -f 2 | cut -d ':' -f 1 | cut -d '{' -f 1))
}

function trains {
	echo -e "\033[0;33m$(wc -l $1)\033[0m"
	for class in ${array[@]}; do
		if [ "$(grep 'class.*{' $1 | cut -d ' ' -f 2 | cut -d ':' -f 1 | cut -d '{' -f 1)" != "$class" ]; then
			val=$(grep $class $1)
			if [ "$val" != "" ]; then
				echo -e "\033[0;31m\t$class\t\033[0;35m$(echo $val)\033[0m"
			fi
		fi
	done
}

files=$(find .. -name *.h)
for f in $files; do
	classes $f
done
for f in $files; do
	trains $f
done
