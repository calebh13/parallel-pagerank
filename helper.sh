#!/bin/bash

for((j = 1; j <= 16; j*= 2)) do
    for ((k = 16; k <= 4096; k = k * 2 )); do
        ./out -k $k -d 0.1 -p $j inputs/web-Google_sorted.txt
    done
done
