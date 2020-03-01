#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Bounce2.h>
#include <Homey.h>

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

  
  //pinMode(THEBUTTON, INPUT); // declare sensor as input (i.e. the doorbell push button)
  pinMode(THEBUTTON, INPUT_PULLUP);

  debouncer.attach(THEBUTTON,INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  debouncer.interval(25); // Use a debounce interval of 25 milliseconds
  
  setupHomey();
}

void setupHomey() {
  Homey.begin("HankersMagischerKnopf");
  //Homey.setClass("trigger");
  Homey.trigger("pressed");
}

// the loop function runs over and over again forever
void loop() {

  Homey.loop();
  
  debouncer.update(); // Update the Bounce instance
   
   if ( debouncer.fell() ) {  // Call code if button transitions from HIGH to LOW
     ledState = !ledState; // Toggle LED state
     digitalWrite(LED_BUILTIN,ledState); // Apply new LED state
     Homey.trigger("pressed", (bool) (ledState ? true : false) );
   }
}
