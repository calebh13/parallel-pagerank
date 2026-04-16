#!/bin/bash

for ((k = 16; k <= 4096; k = k * 2 )); do
    ./out -k $k -d 0.1 -p 12 inputs/web-Google_sorted.txt
done
