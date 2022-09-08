
//Including the following libraries is a must to use the IoT functions and features of esp32
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "analogWrite.h"



const char* ssid = "esp32-wifi@TeamOsiris"; // This will show up when you turn on your mobile WIFI
const char* password = "@perfectlybalanced";  // change to more unique password

AsyncWebServer server(80); // These will start the webserver // If you don't know much, Ignore this line

// These pins are the Enable pins of the L293D motor driver which connects to esp32 gpio pins to implement the PWM function

int enable1_2 = 21; // choose the GPIO pin of esp32;
int enable3_4 = 5 ;// choose the GPIO pin of esp32; 

// These pins are the input pins of l293d on the left side
int inp1 = 1; // Choose your GPIO pin of esp32 for the input 1
int inp2 = 3; // Choose your GPIO pin of esp32 for the input 2


// These pins are the input pins of l293d on the right side
int inp3 = 18; // Choose your GPIO pin of esp32 for the input 3
int inp4 = 19; // Choose your GPIO pin of esp32 for the input 4



int led = 2; // until now you must know what is the inbuilt led pin number of esp32. 

int DutyCycle = 255;
bool x = true;
void setup(){
  // Fill in the blanks
  
  pinMode(enable1_2, OUTPUT);
  pinMode(enable3_4,OUTPUT);


  // The inputs
  // pinMode(int pinNum, MODE) is the function which sets the functional mode of the corresponding pin
  // where first argument is the pin number and the second argument is the mode eg. OUTPUT, INPUT.
  pinMode(inp1, OUTPUT);
  pinMode(inp2, OUTPUT);
  pinMode(inp3, OUTPUT);
  pinMode(inp4, OUTPUT);


  pinMode(led, OUTPUT);


// We use the following function to run the bot at variable speed.
  analogWrite(enable1_2,int(0.95*DutyCycle)); // analog write "255" corresponds to digital write "1"
  analogWrite(enable3_4, DutyCycle);
  
  

  WiFi.softAP(ssid, password);  // This sets your esp32's name as per above mentioned

  IPAddress IP = WiFi.softAPIP();

// A bit of WEB DEV stuff 
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    
    if(request->hasArg("up")){
      // You have to develop the logic that, when the Button "up" on the webpage is pressed, Then the esp32 executes the following commands
      // Refer to the this for the webpage photo
      digitalWrite(inp1, LOW);
      digitalWrite(inp2, HIGH);
      digitalWrite(inp3, HIGH);
      digitalWrite(inp4, LOW);
      digitalWrite(led, HIGH);
    }

    if(request->hasArg("down")){
      digitalWrite(inp1, HIGH);
      digitalWrite(inp2, LOW);
      digitalWrite(inp3, LOW);
      digitalWrite(inp4, HIGH);
      
    }

    if(request->hasArg("left")){
      if (x == false){
        DutyCycle = 170;
      }else{
        DutyCycle = 185;
      }
      DutyCycle = 185;
      analogWrite(enable1_2,int(0.95*DutyCycle));
      digitalWrite(inp1, LOW);
      digitalWrite(inp2, HIGH);
      analogWrite(enable3_4, DutyCycle);
      digitalWrite(inp3, LOW);
      digitalWrite(inp4, HIGH);
    }

    
    
    if(request->hasArg("right")){
      if (x == false){
        DutyCycle = 170;
      }else{
        DutyCycle = 185;
      }
      analogWrite(enable1_2,int(0.95*DutyCycle));
      digitalWrite(inp1, HIGH);
      digitalWrite(inp2, LOW);
      analogWrite(enable3_4, DutyCycle);
      digitalWrite(inp3, HIGH);
      digitalWrite(inp4, LOW);
    }

    
if(request->hasArg("off")){
      if(x){  
        analogWrite(enable1_2, 255);
        analogWrite(enable3_4, 255);
      }else {
        analogWrite(enable1_2, 150);
        analogWrite(enable3_4, 150);
      }
      
      digitalWrite(inp1, LOW);
      digitalWrite(inp2, LOW);
      digitalWrite(inp3, LOW);
      digitalWrite(inp4, LOW);    

      digitalWrite(led, LOW);
    }

if(request->hasArg("slow")){ 
// if '255' is the equivalent to digital '1', and '0' is eqvivalent to digial '0', We vary the pwm values to vary the speed of the motor
      x = false;
      DutyCycle = 150;
      analogWrite(enable1_2, int(0.95*DutyCycle));   
      analogWrite(enable3_4, DutyCycle);  
    }

    if(request->hasArg("fast")){
      x = true;
      DutyCycle = 255;
      analogWrite(enable1_2,int(0.95*DutyCycle));
      analogWrite(enable3_4, DutyCycle);  
    }
    
    request->send_P(200, "text/plain", "");  
  });

  

  server.begin();
}
 
void loop(){
  

}
