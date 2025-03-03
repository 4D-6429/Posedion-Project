#include "LoRa_E22.h"
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(3, 4); //PCB versiyon 4.3 den sonra bu şekilde olmalı
LoRa_E22 FixajSS(&mySerial);
 
#define M0 7
#define M1 6
 
#define Adres 1   //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerden FARKLI olmalı
#define Kanal 20  //Frekans değeri
//E22 için 0--80 arasında bir sayı giriniz. Diğer Modüllerle AYNI olmalı.
//E22 900 için Frekans = 850+kanal değeri.
#define Netid 63  //0--65000 arası bir değer girebilirsiniz. Diğer Modüllerle AYNI olmalı.
 
#define GonderilecekAdres 2
 
struct Signal {
  char sifre[15] = "Fixaj.com";
  byte x[10];
} data;
 
 
void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
 
  Serial.begin(9600);
  FixajSS.begin();
 
  LoraE22Ayarlar();
 
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
 
  delay(500);
  Serial.println("Starting...");
}
 
void loop() {
 
  *(int*)(data.x) = 1234;
  // *(int*)(data.btn3) = 1453;
  // strcpy(data.konum, "Teknofest");
 
  ResponseStatus rs = FixajSS.sendFixedMessage(highByte(GonderilecekAdres), lowByte(GonderilecekAdres), Kanal, &data, sizeof(Signal));
  delay(2000);
}
 
void LoraE22Ayarlar() {
 
  digitalWrite(M0, LOW);
  digitalWrite(M1, HIGH);
 
  ResponseStructContainer c;
  c = FixajSS.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;
 
  //DEĞİŞEBİLEN AYARLAR
  // Üstte #define kısmında ayarlayınız
  configuration.ADDL = lowByte(Adres);
  configuration.ADDH = highByte(Adres);
  configuration.NETID = Netid;
  configuration.CHAN = Kanal;
 
  //SEÇENEKLİ AYARLAR
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;  //Veri Gönderim Hızı 2,4 varsayılan
  //configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;  //Veri Gönderim Hızı 0,3 En uzak Mesafe
  //configuration.SPED.airDataRate = AIR_DATA_RATE_111_625; //Veri Gönderim Hızı 62,5 En Hızlı
 
  configuration.OPTION.subPacketSetting = SPS_240_00;  //veri paket büyüklüğü 240 byte Varsayılan
  //configuration.OPTION.subPacketSetting = SPS_064_10; //veri paket büyüklüğü 64 byte
  //configuration.OPTION.subPacketSetting = SPS_032_11;  //veri paket büyüklüğü 32 en hızlısı
 
  configuration.OPTION.transmissionPower = POWER_22;  //Geönderim Gücü max Varsayılan
  //configuration.OPTION.transmissionPower = POWER_13;
  //configuration.OPTION.transmissionPower = POWER_10;  //Geönderim Gücü min
 
  //GELİŞMİŞ AYARLAR
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011;
  configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED;
 
  //configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_TRANSMITTER;
  configuration.TRANSMISSION_MODE.WORTransceiverControl = WOR_RECEIVER;
 
  //configuration.TRANSMISSION_MODE.enableRSSI = RSSI_ENABLED;
  configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED;
 
  configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION;
  //configuration.TRANSMISSION_MODE.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
 
  configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_DISABLED;
  //configuration.TRANSMISSION_MODE.enableRepeater = REPEATER_ENABLED;
 
  configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED;
  // configuration.TRANSMISSION_MODE.enableLBT = LBT_ENABLED;
 
  // Ayarları KAYDET ve SAKLA
  ResponseStatus rs = FixajSS.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
}