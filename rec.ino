#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define relayPin 6
#define spkrPin 3

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  pinMode(relayPin,OUTPUT);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    String str = String(text);
    Serial.println(str);

    if(str == "Hello World") {
      for(int i = 0; i < 3; i++) {
        tone(spkrPin, 1000);
        delay(100);
        noTone(spkrPin);
        delay(200);
      }
      for(int i = 0; i < 3; i++) {
        tone(spkrPin, 1000);
        delay(50);
        noTone(spkrPin);
        delay(50);
      }

      digitalWrite(relayPin, HIGH);
      delay(1000);
      digitalWrite(relayPin, LOW);
    }
  }

}
