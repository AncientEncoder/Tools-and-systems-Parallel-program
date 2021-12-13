set -e
lengh=10000
threads=16
g++ -fopenmp MergeSort.cpp -o main
clear
echo "--------------------------------------------------------------------------------"
uptime
echo "--------------------------------------------------------------------------------"
echo "Default Mode Test:"
./main $lengh
echo "--------------------------------------------------------------------------------"
echo "Now Take Off:"
 for k in 1000000 2000000 3000000 4000000 5000000 6000000; do
    TimeOne=$(./main $k 1 | grep 'Computation' | awk '{print $4}')
    TimeFour=$(./main $k 4 | grep 'Computation' | awk '{print $4}')
    TimeEight=$(./main $k 8 | grep 'Computation' | awk '{print $4}')
    TimeSixT=$(./main $k 16 | grep 'Computation' | awk '{print $4}')
    TimeThirty=$(./main $k 32 | grep 'Computation' | awk '{print $4}')
    echo "size= ${k}   1 thread = ${TimeOne}s 4 threads= ${TimeFour}s 8 threads= ${TimeEight}s 16 threads= ${TimeSixT}s 32 threads= ${TimeThirty}s"
    done
echo "--------------------------------------------------------------------------------"