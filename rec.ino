#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define relayPin 6
#define spkrPin 3
#define ledPin 4
#define code 9658712

unsigned long previousMillis0 = 0;
unsigned long previousMillis1 = 0;
long ledOnTime = 40;
long ledOffTime = 100;
boolean lightOn = false;




RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
boolean isArmed  = false;

void setup() {
  pinMode(relayPin,OUTPUT);
  pinMode(ledPin, OUTPUT);
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
    if(isArmed) {
      if(str == code) {
        Beep(1000, 5, 200, 200);
        Beep(4000, 3, 50, 50);
        digitalWrite(relayPin, HIGH);
        Success();
        delay(1000);
        digitalWrite(relayPin, LOW);
      }
    }
  }

  unsigned long currentMillis = millis();

  UpdateLights(currentMillis);

}

void Success() {
  const char text[] = "01 (Command Executed)";

  radio.write(&text, sizeof(text));
}

void Arm() {
  isArmed = true;
}


void Beep(int freq, int count, int interval, int duration) {
  for(int i = 0; i < count; i++) {
    tone(spkrPin, freq);
    delay(duration);
    noTone(spkrPin);
    delay(interval);
  }
}

void UpdateLights(unsigned long ms) {

    if(lightOn && ms - previousMillis0 >= ledOnTime) {
      digitalWrite(ledPin, LOW);
      previousMillis0 = ms;
      lightOn = false;
    } else if(!lightOn && ms - previousMillis0 >= ledOffTime) {
      digitalWrite(ledPin, HIGH);
      previousMillis0 = ms;
      lightOn = true;
    }

  }
}

void CheckInput() {

}
