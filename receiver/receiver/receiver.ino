//  4 Channel Receiver | 4 Kanal Alıcı
//  PWM output on pins D2, D3, D4, D5 (Çıkış pinleri)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;

struct Signal {
byte throttle;      
byte pitch;
byte roll;
byte yaw;
};

Signal data;

const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(7, 8); 

void ResetData()
{
// Define the inicial value of each data input. | Veri girişlerinin başlangıç değerleri
// The middle position for Potenciometers. (254/2=127) | Potansiyometreler için orta konum
data.throttle = 127; // Motor Stop | Motor Kapalı
data.pitch = 127;  // Center | Merkez
data.roll = 127;   // Center | Merkez
data.yaw = 127;   // Center | Merkez
}

void setup()
{

  Serial.begin (9600);
  
  //Set the pins for each PWM signal | Her bir PWM sinyal için pinler belirleniyor.
  ch1.attach(2);
  ch2.attach(3);
  ch3.attach(4);
  ch4.attach(5);

  //Configure the NRF24 module
  ResetData();
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  
  radio.startListening(); //start the radio comunication for receiver | Alıcı olarak sinyal iletişimi başlatılıyor
}

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();   // receive the data | data alınıyor
}
}

void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData(); // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
}

/*
ch_width_1 = map(data.throttle, 0, 255, 1000, 2000);     // pin D2 (PWM signal)
ch_width_2 = map(data.pitch,    0, 255, 1000, 2000);     // pin D3 (PWM signal)
ch_width_3 = map(data.roll,     0, 255, 1000, 2000);     // pin D4 (PWM signal)
ch_width_4 = map(data.yaw,      0, 255, 1000, 2000);     // pin D5 (PWM signal)


// Write the PWM signal | PWM sinyaller çıkışlara gönderiliyor
ch1.writeMicroseconds(ch_width_1);
ch2.writeMicroseconds(ch_width_2);
ch3.writeMicroseconds(ch_width_3);
ch4.writeMicroseconds(ch_width_4);
*/
ch1.writeMicroseconds(data.throttle);
ch2.writeMicroseconds(data.pitch);
ch3.writeMicroseconds(data.roll);
ch4.writeMicroseconds(data.yaw);

}
