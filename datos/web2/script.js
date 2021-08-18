
/*
 * declare map as a global variable
 */
var map;
var myMarkers = [];
/*
 * create a initialize function
 */
function initialize() {
    var myCenter=new google.maps.LatLng(51.508742,-0.120850);
    var mapOptions = {
        center: myCenter,
        zoom: 6,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
    };
    
    map = new google.maps.Map(document.getElementById("googleMap"), mapOptions);
    SetMarkers();
}
google.maps.event.addDomListener(window, 'load', initialize);


function SetMarkers() {
    var geocoder = new google.maps.Geocoder();
    var myData = [];
    
    // here you can change this JSON for a call to your database 
    myData = [
        {lat: "51.506767" , lng: "-0.130445", name: "RAFALondon",  address: "RAFALondon, Reino Unido"},
        {lat: "51.754372" , lng: "-1.249592", name: "RAFAOxford",  address: "RAFAOxford, Reino Unido"},
        {lat: "52.137237" , lng: "-0.456837", name: "RAFABedford", address: "RAFABedford, Reino Unido"},
    ];
const svgMarker = {
    path: "M10.453 14.016l6.563-6.609-1.406-1.406-5.156 5.203-2.063-2.109-1.406 1.406zM12 2.016q2.906 0 4.945 2.039t2.039 4.945q0 1.453-0.727 3.328t-1.758 3.516-2.039 3.070-1.711 2.273l-0.75 0.797q-0.281-0.328-0.75-0.867t-1.688-2.156-2.133-3.141-1.664-3.445-0.75-3.375q0-2.906 2.039-4.945t4.945-2.039z",
    fillColor: "blue",
    fillOpacity: 0.6,
    strokeWeight: 0,
    rotation: 0,
    scale: 2,
    anchor: new google.maps.Point(15, 30),
  };
    
    for(i = 0; i < myData.length; i++) {
        geocoder.geocode({ 'address': myData[i].address }, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {                
                myMarkers[i] = new google.maps.Marker({
                    position: results[0].geometry.location,
			icon: svgMarker,
                    map: map
                });
            } else {
                console.log("We can't found the address, GoogleMaps say..." + status);
            }
        });
    }
}


