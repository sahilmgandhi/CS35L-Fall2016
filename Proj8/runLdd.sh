#!/bin/bash

outputs=$(ls /usr/bin | awk 'NR%101==704598105%101')

for line in $outputs; do
	ldd /usr/bin/$line >> temp.txt
done

sort -u temp.txt > outputs.txt