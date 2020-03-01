#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Bounce2.h>

#include <Homey.h>

//connect one end of the doorbell push button lead to D2,
//connect a 100K resistor from D2 to positive 3V3,
//connect the other end of the doorbell push button lead to ground.

int THEBUTTON = 4; 
int ledState = LOW;
Bounce debouncer = Bounce(); // Instantiate a Bounce object

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  //Connect to network
  WiFi.begin("BabyStube", "PaulaNadja1977");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi up");
  //Print IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Homey.begin("Doorbell"); // this will be the name of your button in Homey
  Serial.println("Homey.begin()");
  Homey.setClass("socket"); // this is the classtype. The doorbell push button behaves like a socket with on/off functionality
  Serial.println("Homey.setClass()");
  //Homey.addCapability("onoff", onoffCb); // adding capability _onoff to Trigger Boolean
  Serial.println("Homey.addCapability()");
  
  //pinMode(THEBUTTON, INPUT); // declare sensor as input (i.e. the doorbell push button)
  pinMode(THEBUTTON, INPUT_PULLUP);

  debouncer.attach(THEBUTTON,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  digitalWrite(LED_BUILTIN,ledState);
}

// the loop function runs over and over again forever
void loop() {

  Homey.loop();
  
  //int doorbellState = digitalRead(THEBUTTON);

  debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     digitalWrite(LED_BUILTIN,ledState); // Apply new LED state
   }
}

  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  //Serial.write("on");
  //delay(1000);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //Serial.write("off");
  //delay(1000);                       // wait for a second
