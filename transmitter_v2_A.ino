#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E2LL;
RF24 radio(9, 10);

float sent_data_1A[] = {0, 0};

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  radio.openWritingPipe(my_radio_pipe);
  radio.setRetries(1, 15);
  sent_data_1A[0] = 0;
  sent_data_1A[1] = 0;

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop()
{
  sent_data_1A[0] = analogRead(A0) * (5.0 / 1023.0);
  sent_data_1A[1] = 100 + analogRead(A1) * (5.0 / 1023.0);
  Serial.print(sent_data_1A[0]);
  Serial.print("\t");
  Serial.print(sent_data_1A[1]);
  Serial.print("\n");
  if (sent_data_1A[0] > 3) {
    radio.write(&sent_data_1A[0], sizeof(sent_data_1A[0]));
  }
  if (sent_data_1A[1] > 103) {
    radio.write(&sent_data_1A[1], sizeof(sent_data_1A[1]));
  }
}
