#!/bin/bash

next_seq_begin=0
seq_end=0
seq_begin=0

for line in `cat ./errblk.txt |grep -v "#"`
do
	cur_seq_begin=`echo $line | awk -F'-' '{ print $1 }'`
	cur_seq_end=`echo $line | awk -F'-' '{ print $2 }'`
	
	if [ ! $(($cur_seq_begin)) -eq $(($next_seq_begin)) ]
	then
		echo "SEQ BEGIN: $seq_begin   SEQ END: $seq_end    DIST= $(( $seq_end -$seq_begin ))" 
		seq_begin=$cur_seq_begin 
	fi	
	
	next_seq_begin=$(( $cur_seq_end + 1 )) 
	seq_end=$cur_seq_end; 

done

echo "LAST  SEQ BEGIN: $seq_begin   SEQ END: $seq_end    DIST= $(($seq_end - $seq_begin))" 
