#! /bin/sh

# 查看文件被哪些进程打开

if [ $# -ne 1 ]
then
	echo "usage: $0 file"
	exit -1
fi

filePath=""
if [[ $1 == /* ]]
then
	filePath=$1	
else
	filePath=`pwd`/$1	
fi

selfPid=$$
allpids=`ps -e -opid`
allpids=${allpids:6}	# 斩断开头的"  PID " 

for pid in $allpids
do
	if [ $pid -eq $selfPid ]	# 排除自身
	then
		continue
	fi

	if [ -d /proc/$pid/fd ]
	then
		for file in /proc/$pid/fd/*
		do
			if [ -L $file ]
			then
				lsret=`ls -l $file`
				if [[ $lsret == *$filePath ]]
				then
					ps -p $pid -opid,cmd
					break
				fi
			fi
		done
	fi
done

exit 0

