#1/bin/bash
name=0
fname="./"
for files in $(ls ./*.jpg)
	do
		hname=".jpg"
		name=$(echo "$name + 1"|bc)
		filename=$fname$name$hname
		mv $files $filename
		#echo $files$filename
	done
