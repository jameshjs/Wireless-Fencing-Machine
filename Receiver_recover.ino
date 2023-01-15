#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn = 0xE8E8F0F0E2LL;
RF24 radio(2, 3);

float dataOne = 0;
float dataGNDOne = 0;
float dataTwo = 0;
float dataGNDTwo = 0;
float receiveData = 0;
boolean activation = false;
boolean OneLight=false;
boolean TwoLight=false;
boolean OneLightGND=false;
boolean TwoLightGND=false;
boolean OneActivate=false;
boolean TwoActivate=false;
boolean firstLightActivate=true;
boolean secondLightActivate=true;
unsigned long currentTime = 0;
unsigned long interval = 0;


void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
}

//unsigned long last_Time = 0;

void receivingData() {
  while ( radio.available() ) {
    radio.read(&receiveData, sizeof(receiveData));
    if (receiveData > 3 && receiveData < 100) {
      dataOne = receiveData;
    }
    else if (receiveData > 103 && receiveData < 200) {
      dataGNDOne = receiveData;
    }
    else if (receiveData > 203 && receiveData < 300) {
      dataTwo = receiveData;
    }
    else if (receiveData > 203 && receiveData < 300) {
      dataTwo = receiveData;
    }
  }
}

void assignSignal() {
  if (activation == false) {
    if (dataOne > 3) {
      if (dataGNDTwo > 3) {
        OneLightGND = true;
      }
      else if (dataGNDTwo < 3) {
        OneActivate = true;
      }
    }
    else if (dataTwo > 3) {
      if (dataGNDOne > 3) {
        TwoLightGND = true;
      }
      else if (dataGNDOne < 3)
        TwoActivate = true;
    }
    if (OneActivate == true || TwoActivate == true) {
      activation = true;
      currentTime = millis();
    }
  }
  else if (activation == true) {
    interval = millis - currentTime;
    if (firstLightActivate == true) {
      if (OneActivate == true ) {
        OneLight = true;
        firstLightActivate = false;
      }
      else if (TwoActivate == true) {
        TwoLight = true;
        firstLightActivate = false;
      }
    }
    if (interval < 1000) {
      secondLightActivate = true;
    }
    if (secondLightActivate == true) {
      if (OneLight == true) {
        if (dataOne > 3 && dataGNDTwo < 3) {
          TwoLight = true;
        }
      }
      else if (TwoLight == true) {
        if (dataTwo > 3 && dataGNDOne < 3) {
          OneLight = true;
        }
      }
    }

    if (interval > 1000) {
      activation = false;
      OneActivate = false;
      TwoActivate = false;
      firstLightActivate = true;
      secondLightActivate = true;
    }
  }
}

void signalHit() {
  if (OneLightGND == true) {
    Serial.println((String)"#1 GND");
  }
  else if (TwoLightGND == true) {
    Serial.println((String)"#2 GND");
  }
  if (OneLight == true && TwoLight == true) {
    Serial.println((String)"DOUBLE");
  }
  else {
    if (OneLight == true) {
      Serial.println((String)"#1 SIGNLE");
    }
    else if (TwoLight == true) {
      Serial.println((String)"#2 SINGLE");
    }
  }
  OneLightGND = false;
  TwoLightGND = false;
  OneLight = false;
  TwoLight = false;
}

void loop() {
  receivingData();
  assignSignal();
  signalHit();
}
