
function initMap() {
  const myLatLng = { lat: -38.8300240 , lng: -68.0673740 };
  const map = new google.maps.Map(document.getElementById("map"), {
    zoom: 18,
    center: myLatLng,
  });
  new google.maps.Marker({
    position: myLatLng,
    map,
    title: "Hello World!",
  });
}
