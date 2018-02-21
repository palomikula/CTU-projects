#!/bin/bash

while getopts ":hiu:" opt; do
	case $opt in
	h) echo "strucna napoveda"
		exit 0
	;;

	i)	file="`tr -d ''`"
		echo ${file[*]} >&2
		echo ${file[*]} | tr -d '\n' | grep -i -E '<[[:blank:]]*a +' | grep -o -i -E ' href[[:blank:]]*=[[:blank:]]*"[^"]*"[>"]*>' | grep -i -E '\.pdf"' | sed -e 's/^.*href/href/' | grep -o -i -E '"[^"]*"' | tr -d '"'
		exit 0
	;;
	
	u) file="`wget -qO- "${OPTARG}"`"
	   echo ${file[*]} | tr -d '\n' |grep -i -E '<[[:blank:]]*a +' | grep -o -i -E ' href[[:blank:]]*=[[:blank:]]*"[^"]*"[>"]*>' | grep -i -E '\.pdf"' | sed -e 's/^.*href/href/' | grep -o -i -E '"[^"]*"' | tr -d '"'
	   exit 2
	;;

	?) exit 2
	;;
	esac
done

exit 1
