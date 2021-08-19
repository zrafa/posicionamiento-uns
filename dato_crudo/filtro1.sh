#!/bin/bash 

wifi=2-wifi.txt 
gps=2-gps.txt 

for marca in $(egrep -e "^[0-9]{10}" $wifi |sort -u );do 
   # sed -n "/$marca/,/----/p" $gps  > arch.$marca
   sed -n "/$marca/,/----/p" $wifi  > arch.$marca
   grep -q Cell arch.$marca || continue  
   grep -A1 $marca  $gps  >> arch.$marca
   echo Estampilla tiempo: $marca
   egrep  -e "GNRMC" arch.$marca |cut -f 4,6 -d,
   egrep -o --color  -e '(([[:xdigit:]]{2}):){5}[[:xdigit:]]{2}' -e "level=-[0-9]*" arch.$marca |sed 's/level=-//' | paste - -
done 

rm arch.*
