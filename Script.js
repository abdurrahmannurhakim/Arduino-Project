/*
============================================================================
 Name        : Script.js
 Author      : Abdurrahman Nurhakim
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : Simple Code using JavaScript for Processing Fuel Range Value from MCU 
============================================================================
*/

setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("temp").innerHTML = this.responseText;
      }
    };
    location.reload();
    xhttp.open("GET", "/temp", true);
    xhttp.send();
  }, 1000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("humidity").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
  }, 1000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("voltage").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/voltage", true);
    xhttp.send();
  }, 1000 ) ;

  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("fuel").innerHTML = this.responseText;
      }
    };
    xhttp.open("GET", "/fuel", true);
    xhttp.send();
  }, 1000 ) ;



  
