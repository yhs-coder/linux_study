while :; 
do ps ajx |  head -1 && ps ajx | grep orphan_process | grep -v grep; 
sleep 1;
echo "----------------------------------------------------------------";
done
