file_names=("101.txt" "501.txt" "1001.txt" "2001.txt" "3001.txt" "5001.txt" "7501.txt" "10001.txt" "15001.txt" "20001.txt")
for value in ${file_names[*]}
do
	./perfectSquareSequential.exe "inputs/i"$value
	source bench_sub.sh "inputs/i"$value 2> "seqTest/Timesi"$value
done
for value in ${file_names[*]}
do
	./perfectSquareSequential.exe "inputs/r"$value
	source bench_sub.sh "inputs/r"$value 2> "seqTest/Timesr"$value
done
for value in ${file_names[*]}
do
	./perfectSquareSequential.exe "inputs/p"$value
	source bench_sub.sh "inputs/p"$value 2> "seqTest/Timesp"$value
done