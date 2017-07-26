cd cross_compile

for i in {1..4}
do
  ./HPXparallel -t $i --cores $i --trials 100000 --packets 1000
done

now=$(date +"%Y-%m-%d-%S")

mv HPXparData.csv HPXparData_$now.csv

scp HPXparData_$now.csv jgoncal@130.39.95.131:/home/jgoncal/repos/MonteCarloPi/data

rm HPXparData_$now.csv

exit
