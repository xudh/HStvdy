#! /bin/sh

# 查看文件被哪些进程打开

if [ $# -ne 1 ]
then
	echo "usage: $0 file/directory"
	exit -1
fi

filePath=""
if [[ $1 == /* ]]
then
	filePath=$1	
else
	filePath=`pwd`/$1	
fi

if [ ! -e $filePath ]
then
	echo "$filePath not exist"
	exit -1
fi

fileType=0			# 0/1/2分别表示文件/文件夹/文件夹下任何文件
if [[ $filePath == */ ]]
then
	fileType=2
elif [ -d $filePath ]
then
	fileType=1
fi

selfPid=$$
allpids=`ps -e -opid`
allpids=${allpids:6}	# 斩断开头的"  PID " 

matched=0
for pid in $allpids
do
	if [ $pid -eq $selfPid ]	# 排除自身
	then
		continue
	fi

	if [ -d /proc/$pid/fd ]
	then
		bMatch=0
		for file in /proc/$pid/fd/*
		do
			if [ -L $file ]
			then
				lsret=`ls -l $file`
				if [ $fileType -eq 2 ]	# 文件夹下的任意文件
				then
					if [[ $lsret == *$filePath* ]]
					then
						bMatch=1
						break
					fi
				else	# 文件或文件夹本身
					if [[ $lsret == *$filePath ]]
					then
						bMatch=1
						break
					fi
				fi
			fi
		done
		if [ $bMatch -eq 1 ]
		then
			if [ $matched -eq 0 ]
			then
				ps -p $pid -opid,cmd
			else
				ps -p $pid -opid,cmd | tail -n 1
			fi
			matched=1
		fi
	fi
done

exit 0

