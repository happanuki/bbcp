#!/bin/bash
#set -x

source setup.me

_RESULT=`losetup -l | grep $DISK_FILENAME`

set -e # turning exitcode control on due to grep

if [ "$_RESULT" != "" ] 
then
	echo "Already attached as $_RESULT"
	exit 0;
else
loopDev=$(sudo kpartx -av $DISK_FILENAME | awk '{print $3;}')

echo "mapped file :  /dev/mapper/$loopDev "

mkdir -p $MOUNT_POINT

fi

