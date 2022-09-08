
// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include "analogWrite.h"

// Replace with your network credentials
const char* ssid = "Yamete";
const char* password = "RBD40122";

int enableA = 21; // choose the GPIO pin of esp32;
int enableB = 5 ;// choose the GPIO pin of esp32; 

// These pins are the input pins of l293d on the left side
int inp1 = 1; // Choose your GPIO pin of esp32 for the input 1
int inp2 = 3; // Choose your GPIO pin of esp32 for the input 2


// These pins are the input pins of l293d on the right side
int inp3 = 18; // Choose your GPIO pin of esp32 for the input 3
int inp4 = 19; // Choose your GPIO pin of esp32 for the input 4
 
int led = 2; // until now you must know what is the inbuilt led pin number of esp32. 

int DutyCycle = 255;

String sliderValue= "0";

const char* PARAM_INPUT = "value";

int l1,l2,l,r1,r2,r,a,b;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<html>
<head>
    <meta name="viewport" content="width=device-width">
</head>
<style>
  .unselectable {
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
}

input[type="range"] {
  -webkit-appearance: slider-vertical;
  height:225px;
  width:250px;
}

</style>



  <div style="float:left;"> 
    <input type="range" id="Left" name="Left" min="10" max="70" ontouchmove="slider_move()" ontouchend="slider_off_l()">
  </div>


  <div style="float:right;">
    <input type="range" id="Right" name="Right" min="10" max="70" ontouchmove="slider_move()" ontouchend="slider_off_r()">
  </div>




<div style="text-align:center;">
        <button class="unselectable" ontouchstart="Creep_forward()" ontouchend="slider_off_both()" style="height:100px;width:100px;"> Creep Forward </button>                                                    </div>
</div>

<p id="demo" style="text-align:center;"> Amogus </p>


<div style="text-align:center;">
  <button class="unselectable" ontouchstart="Emergency_stop()" ontouchend="slider_off_both()" style="height:100px;width:100px;"> Emergency Stop </button>
</div>


<script>
    var slider_l = document.getElementById("Left").value;
    var slider_r = document.getElementById("Right").value;


    function Emergency_stop(){
     document.getElementById("Left").value = 10;
     document.getElementById("Right").value = 10;
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }
    
    function Creep_forward(){
     document.getElementById("Left").value = 55;
     document.getElementById("Right").value = 55;
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }

    function slider_move(){
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }


    function slider_off_l(){
     document.getElementById("Left").value = 40;
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }

    function slider_off_r(){
     document.getElementById("Right").value = 40;
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }

    function slider_off_both(){
     document.getElementById("Left").value = 40;
     document.getElementById("Right").value = 40;
     var x = document.getElementById("Left").value;
     var y = document.getElementById("Right").value;
     var m = x - 40;
     var n = y - 40;
     document.getElementById("demo").innerHTML = "L: " + m + " R: " + n;
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/control?value="+x+y, true);
     xhr.send();
    }
</script>
</body>
</html>

)rawliteral";

// Replaces placeholder with button section in your web page

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200); 
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);


  // The inputs
  // pinMode(int pinNum, MODE) is the function which sets the functional mode of the corresponding pin
  // where first argument is the pin number and the second argument is the mode eg. OUTPUT, INPUT.
  pinMode(inp1, OUTPUT);
  pinMode(inp2, OUTPUT);
  pinMode(inp3, OUTPUT);
  pinMode(inp4, OUTPUT);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  analogWrite(enableA, 255);
  analogWrite(enableB, 255);
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/control", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    l1 = char(sliderValue[0])-48;
    l2 = char(sliderValue[1])-48;
    l = l1*10+l2 - 40;
    
    r1 = char(sliderValue[2])-48;
    r2 = char(sliderValue[3])-48;
    r = r1*10+r2 - 40;
    a = 255*l/30;
    b = 255*r/30;
    Serial.println(a);
    Serial.println(b);
    analogWrite(enableA, a);
    analogWrite(enableB, b);
    if(l*r > 0){
      if(l>0){
        digitalWrite(inp1, LOW);
        digitalWrite(inp2, HIGH);
        digitalWrite(inp3, HIGH);
        digitalWrite(inp4, LOW);
      }
      else{
        digitalWrite(inp1, HIGH);
        digitalWrite(inp2, LOW);
        digitalWrite(inp3, LOW);
        digitalWrite(inp4, HIGH);
      }
    }
    else if(l*r < 0){
      if(l>0){
        digitalWrite(inp1, HIGH);
        digitalWrite(inp2, LOW);
        digitalWrite(inp3, HIGH);
        digitalWrite(inp4, LOW);
      }
      else{
        digitalWrite(inp1, LOW);
        digitalWrite(inp2, HIGH);
        digitalWrite(inp3, LOW);
        digitalWrite(inp4, HIGH);
      } 
    }
    else{
      if(l == 0){
        if(r>0){
          digitalWrite(inp1, LOW);
          digitalWrite(inp2, HIGH);
          digitalWrite(inp3, HIGH);
          digitalWrite(inp4, LOW);
        }
        else{
          digitalWrite(inp1, HIGH);
          digitalWrite(inp2, LOW);
          digitalWrite(inp3, LOW);
          digitalWrite(inp4, HIGH);
        }
      }
      else{
        if(l>0){
          digitalWrite(inp1, LOW);
          digitalWrite(inp2, HIGH);
          digitalWrite(inp3, HIGH);
          digitalWrite(inp4, LOW);
        }
        else{
          digitalWrite(inp1, HIGH);
          digitalWrite(inp2, LOW);
          digitalWrite(inp3, LOW);
          digitalWrite(inp4, HIGH);
        }
      }
    }

    
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  

}
