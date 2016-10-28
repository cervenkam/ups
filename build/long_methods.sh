#!/bin/bash
cmpr=0
function long_methods {
	echo -e "\033[0;33m$(wc -l $1)\033[m"
	text11=$(grep -n "^[A-Za-z].*{$" $1  | cut -d ':' -f 1)
	text21=$(grep -n "^}" $1  | cut -d ':' -f 1)
	text12=$(grep -n "^[A-Za-z].*{$" $1 | tr ' ' '_' | cut -d ':' -f 1 --complement)
	arr1=($text11)
	arr2=($text21)
	arr3=($text12)
	if [  ${#arr1[@]} -ne ${#arr2[@]} ]; then
		echo -e "\033[0;36mBad file\033[0m"
		echo $text11
		echo $text21
		exit;
	fi
	for((a=0; a<${#arr1[@]}; a++)); do
		value=$(echo "${arr2[$a]} - ${arr1[$a]} + 1" | bc)
		if [ "$2" != "" ]; then
			cmpr=$2
		fi
		if [ $value -ge $cmpr ]; then 
			echo -e "\033[0;31m$value\033[0;35m: $(echo ${arr3[$a]} | tr '_' ' ' | cut -d '{' -f 1)\033[0m"
		fi
	done
}

if [ "$1" != "" ]; then
	long_methods $*
	exit
fi

