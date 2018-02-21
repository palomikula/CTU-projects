#!/bin/bash
while getopts ":hvzn" opt; do
	case $opt in
	h) echo "strucna napoveda"
		exit 0
	;;
	v) F=0 D=0 H=0
		o1=v
	;;
	z) files=()
		o2=z
	;;
	n) curF=0 curD=0 curH=0
		o3=n
	;;
	?) exit 2
	;;
	esac
done

ret=0

while read line; do
	echo "Ctu radku '$line'" >&2
	if [ "${line:0:5}" = "FILE " ]; then
		file=${line#*"FILE "}
		if [ -h "$file" ]; then
			printf "LINK "
			if [ "$o1" = "v" ]; then 
				let H++ 
			fi

			if [ "$o3" = "n" ]; then 
				let curH++ 
				printf "%s$curH "
			fi
			echo "'${file}'" "'$(readlink "${file}")'"

		elif [ -d "$file" ]; then
			printf "DIR "
			if [ "$o1" = "v" ]; then 
				let D++ 
			fi

			if [ "$o3" = "n" ]; then 
				let curD++ 
				printf "%s$curD "
			fi
			echo "'${file}'"

		elif [ -f "$file" ]; then
			printf "FILE "
			if [ "$o1" = "v" ]; then 
				let F++ 
			fi

			if [ "$o2" = "z" ]; then 
				files+=("${file}") 
			fi

			if [ "$o3" = "n" ]; then 
				let curF++ 
				printf "%s$curF "
			fi
			echo "'${file}'" $(wc -l < "${file}") "'$(head -n 1 < "${file}")'"

		else
			>&2 echo "ERROR '${file}'"
			ret=1
		fi
	fi
done
if [ "$o1" = "v" ]; then 
	echo $F
	echo $D
	echo $H 
fi
if [ "$o2" = "z" ]; then 
	tar czf output.tgz "${files[@]}"
fi


exit $ret
