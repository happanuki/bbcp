#!/bin/bash
set -x
set -e

from=$1
to=$2

[[ -f ddpt.pid || -f cat.pid ]] && { echo "old pids are present \"*.pid\" remove first"; exit 1; }

[[ $USER != root ]] && { echo "Should be root"; exit 1; }
[[ ! $from || ! $to ]] && { echo " usage: $0 <dev-file-from> <dev-file-to>" ; exit 1; }

pipe=./pipe
[[ ! -f $pipe ]] && { mkfifo $pipe || ( echo "could not create pipe" && exit 1 ); }

touch errblk.txt || ( echo "could not create errblk.txt" && exit 1 ) 

nohup ddpt bpt=128 bs=512 if=$from iflag=pt,coe,fua,direct,errblk of=$pipe -v > ddpt.console.log 2>&1 &
echo $! > ddpt.pid


cat $pipe | gzip > $to &
echo $! > cat.pid

#tail -f ddpt.console.log
echo "wait..."
wait

