#!/bin/bash

source setup.me

pushd $MOUNT_POINT

files=$(ls /usr/share | tail -10)
for f in $files; do
	cp -rv /usr/share/$f ./
done

popd
