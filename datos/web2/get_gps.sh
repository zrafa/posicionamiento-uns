#!/bin/bash

for i in $(seq 2 42); do
	cat script.js0 > /tmp/script.js
	grep "web movil${i}:" resultado.txt | sed -e "s/web movil${i}://" >> /tmp/script.js
	cat script.js1 >> /tmp/script.js
	cp /tmp/script.js script.js
	sleep 5
done
