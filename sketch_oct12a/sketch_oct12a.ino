/*
 ESP8266 Blink by Idayu Sabri
 Blink LED Using ESP8266 NodeMCU Lua WiFi
 Note that this sketch uses BUILTIN_LED to find the pin with the internal LED
*/
// BI -> 2

// D2 -> 4
// D3 -> 0;
// D4 -> 2; ??

int led_num = 10;
int from = 0;
int to = 5;

void setup() {
  pinMode(0, OUTPUT);     // Initialize the BUILTIN_LED pin as an output 

  for(int i=from; i< to; i++){
    pinMode(i, OUTPUT); 
    }
}

// the loop function runs over and over again forever
void loop() {
  
    for(int i=from; i< to; i++){

      digitalWrite(i, LOW); 
      delay(200);                  
      digitalWrite(i, HIGH);  
      delay(200); 
    }
  delay(400); 
}
