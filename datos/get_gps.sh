#!/bin/bash

grep GNRMC *gps.txt | while read l; do
	LAT1=$(echo $l | cut -d, -f4 | sed -e 's/.\{2\}/& /' | cut -d\  -f1)
	LAT2=$(echo $l | cut -d, -f4 | sed -e 's/.\{2\}/& /' | cut -d\  -f2)
	LAT=$(echo "$LAT1 + ($LAT2/60)" | bc -l | cut -c1-10)
	LONG1=$(echo $l | cut -d, -f6 | sed -e 's/.\{3\}/& /' | cut -d\  -f1)
	LONG2=$(echo $l | cut -d, -f6 | sed -e 's/.\{3\}/& /' | cut -d\  -f2)
	LONG=$(echo "$LONG1 + ($LONG2/60)" | bc -l | cut -c1-10)
	# echo  "$LAT1  $LAT2"
	echo -${LAT},-${LONG}
	MAP="https://www.google.com/maps/@-${LAT},-${LONG},15z"
	echo $MAP
	MAP2="https://maps.google.com/?ll=-${LAT},-${LONG}&z=14&t=m&output=embed"
	echo $MAP2
#	LAT=$(echo $l | cut -d, -f4 | sed -e "s/\.//" -e 's/.\{2\}/& /')
#	LONG1=$(echo $l | cut -d, -f6 | sed -e "s/\.//" -e 's/.\{3\}/& /')
#	LONG=$(echo $l | cut -d, -f6 | sed -e "s/\.//" -e 's/.\{3\}/& /')
#	echo  "-$LAT  -$LONG"
#	LAT=$(echo $l | cut -d, -f4 | sed -e "s/\.//" -e 's/.\{2\}/&./')
#	LONG=$(echo $l | cut -d, -f6 | sed -e "s/^0//" -e "s/\.//" -e 's/.\{2\}/&./')
#	echo  "-$LAT  -$LONG"

echo '
function initMap() {
  const myLatLng = { lat: -'$LAT' , lng: -'$LONG' };
  const map = new google.maps.Map(document.getElementById("map"), {
    zoom: 18,
    center: myLatLng,
  });
  new google.maps.Marker({
    position: myLatLng,
    map,
    title: "Hello World!",
  });
}' > /tmp/index.js

	cp /tmp/index.js web/

	sleep 10
done 

