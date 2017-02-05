#!/bin/bash
set -e

source setup.me

[[ -f ${DISK_FILENAME} ]] && { echo "${DISK_FILENAME} already exists, exiting"; exit 0; } || { :; }

dd if=/dev/zero of=${DISK_FILENAME} bs=1M count=200

fdisk ${DISK_FILENAME} << -EOI
n
p



t
7
p
w
-EOI

echo "DONE"
