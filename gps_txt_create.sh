cat gps-er.txt | grep -v , | cut -d" " -f1 | sort -u > gps.txt
