while :; 
do 
	ps ajx | head -1 && ps ajx | grep -E 'zombie_process|orphan_process' | grep -v grep;
	sleep 1;
	echo "--------------------------------------------------------------------------";
done
