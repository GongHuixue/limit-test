# Copyright(c) 2016-2100.  fanygong.  All rights reserved.
#
#   FileName:     cpu.sh
#   Author:       fanygong
#   Email:        493164984@qq.com
#   DateTime:     2019-07-26 19:41:22
#   ModifiedTime: 2019-07-26 19:41:22

JLLPATH="$(which $0)"
JLLPATH="$(dirname ${JLLPATH})"
#source ${JLLPATH}/BashShellLibrary

### Color Echo Usage ###
# Lfn_Sys_ColorEcho ${CvFgRed} ${CvBgWhite} "hello"
# echo -e "hello \033[0m\033[31m\033[43mworld\033[0m"

#! /bin/sh  
# filename killcpu.sh 
if [ $# != 1 ] ; then 
  echo "USAGE: $0 <CPUs>"
  exit 1; 
fi
for i in `seq $1` 
do
  echo -ne "  
i=0;  
while true 
do 
i=i+1;  
done" | /bin/sh & 
  pid_array[$i]=$! ; 
done

time=$(date "+%Y-%m-%d %H:%M:%S")
echo "${time}"

for i in "${pid_array[@]}"; do
  echo 'kill ' $i ';'; 
done

sleep 60

for i in "${pid_array[@]}"; do
  kill $i;
done
