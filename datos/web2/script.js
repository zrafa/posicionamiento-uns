
/*
 * declare map as a global variable
 */
var map;
var myMarkers = [];
/*
 * create a initialize function
 */
function initialize() {
    var myCenter=new google.maps.LatLng(-38.8281786,-68.0700259);
    var mapOptions = {
        center: myCenter,
        zoom: 19,
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
 {lat: -38.827751 , lng: -68.069695, name: "knn",  address: "knn"}, 
 {lat: -38.827770 , lng: -68.069664, name: "maf",  address: "maf"}, 
 {lat: -38.827755 , lng: -68.069679, name: "cdb",  address: "cdb"}, 
 {lat: -38.827766 , lng: -68.069664, name: "cdb",  address: "cdb"}, 
    ];
const marca0 = {
    path: "M10.453 14.016l6.563-6.609-1.406-1.406-5.156 5.203-2.063-2.109-1.406 1.406zM12 2.016q2.906 0 4.945 2.039t2.039 4.945q0 1.453-0.727 3.328t-1.758 3.516-2.039 3.070-1.711 2.273l-0.75 0.797q-0.281-0.328-0.75-0.867t-1.688-2.156-2.133-3.141-1.664-3.445-0.75-3.375q0-2.906 2.039-4.945t4.945-2.039z",
    fillColor: "blue",
    fillOpacity: 0.6,
    strokeWeight: 0,
    rotation: 0,
    scale: 2,
    anchor: new google.maps.Point(15, 30),
  };
const marca1 = {
    path: "M10.453 14.016l6.563-6.609-1.406-1.406-5.156 5.203-2.063-2.109-1.406 1.406zM12 2.016q2.906 0 4.945 2.039t2.039 4.945q0 1.453-0.727 3.328t-1.758 3.516-2.039 3.070-1.711 2.273l-0.75 0.797q-0.281-0.328-0.75-0.867t-1.688-2.156-2.133-3.141-1.664-3.445-0.75-3.375q0-2.906 2.039-4.945t4.945-2.039z",
    fillColor: "red",
    fillOpacity: 0.6,
    strokeWeight: 0,
    rotation: 0,
    scale: 2,
    anchor: new google.maps.Point(15, 30),
  };
    
const marca2 = {
    path: "M10.453 14.016l6.563-6.609-1.406-1.406-5.156 5.203-2.063-2.109-1.406 1.406zM12 2.016q2.906 0 4.945 2.039t2.039 4.945q0 1.453-0.727 3.328t-1.758 3.516-2.039 3.070-1.711 2.273l-0.75 0.797q-0.281-0.328-0.75-0.867t-1.688-2.156-2.133-3.141-1.664-3.445-0.75-3.375q0-2.906 2.039-4.945t4.945-2.039z",
    fillColor: "green",
    fillOpacity: 0.6,
    strokeWeight: 0,
    rotation: 0,
    scale: 2,
    anchor: new google.maps.Point(15, 30),
  };
    
const marca3 = {
    path: "M10.453 14.016l6.563-6.609-1.406-1.406-5.156 5.203-2.063-2.109-1.406 1.406zM12 2.016q2.906 0 4.945 2.039t2.039 4.945q0 1.453-0.727 3.328t-1.758 3.516-2.039 3.070-1.711 2.273l-0.75 0.797q-0.281-0.328-0.75-0.867t-1.688-2.156-2.133-3.141-1.664-3.445-0.75-3.375q0-2.906 2.039-4.945t4.945-2.039z",
    fillColor: "black",
    fillOpacity: 0.6,
    strokeWeight: 0,
    rotation: 0,
    scale: 2,
    anchor: new google.maps.Point(15, 30),
  };
    
    
 new google.maps.Marker({
    position: myData[0],
    map,
    title: "Hello World0!",
    icon: marca0,
  });
 new google.maps.Marker({
    position: myData[1],
    map,
    icon: marca1,
    title: "Hello World1!",
  });
 new google.maps.Marker({
    position: myData[2],
    map,
    icon: marca2,
    title: "Hello World2!",
  });
 new google.maps.Marker({
    position: myData[3],
    map,
    icon: marca3,
    title: "Hello World3!",
  });


}


