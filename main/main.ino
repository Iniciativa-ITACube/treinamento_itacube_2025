#include <Arduino.h>
#include "HT_SSD1306Wire.h"

// Subsistemas
#include "NAV.h"
#include "EPS.h"
#include "OBDH.h"
#include "PAY.h"
#include "TTeC.h"
#include "images.h"
#include "Pins.h"

// Instâncias dos sensores e módulos
NEO6M gps;
Battery battery;
MQ135 mq135;
GUVA guva;
BME280Sensor bme280;
MPU6050Sensor imu;
SDCardHandler sd;
LORA32 lora;
uint16_t msgId = 0;

SSD1306Wire factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);

void logo() {
  factory_display.clear();
  factory_display.drawXbm(0, 5, logo_width, logo_height, (const unsigned char *)logo_bits);
  factory_display.display();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicialização do display e logo
  factory_display.init();
  factory_display.clear();
  logo();
  delay(3000);

  // Inicialização dos módulos
  Serial.println("Inicializando módulos...");
  gps.gpsBegin();
  battery.begin();
  mq135.begin();
  guva.begin();
  bme280.begin();
  imu.begin();
  sd.begin();
  lora.begin();
  Serial.println("Todos os módulos iniciados.");
  sd.logLine( "temp,hum,press,uv,co2_v,vbatt,lat,lon,alt,sats,ax,ay,az,gx,gy,gz");
}

int mysteriousIntType = 0;
void loop() {
  // Atualização dos sensores
  gps.gpsUpdate();
  imu.update();

  // Leitura dos sensores com fallback para "N/A"
  float temp = bme280.readTemperature();
  float press = bme280.readPressure();
  float hum = bme280.readHumidity();

  float co2_v = mq135.readVoltage();
  float uv = guva.readUV();
  float vbatt = battery.readVoltage();

  float ax = imu.getAcX();
  float ay = imu.getAcY();
  float az = imu.getAcZ();
  float gx = imu.getGyX();
  float gy = imu.getGyY();
  float gz = imu.getGyZ();
  double lat_d = gps.gps.location.isValid() ? gps.gps.location.lat() : 0.0;
  double lon_d = gps.gps.location.isValid() ? gps.gps.location.lng() : 0.0;
  double alt_d = gps.gps.location.isValid() ? gps.gps.altitude.meters() : 0.0;
  String lat = gps.gps.location.isValid() ? String(lat_d, 6) : "N/A";
  String lon = gps.gps.location.isValid() ? String(lon_d, 6) : "N/A";
  String alt = gps.gps.location.isValid() ? String(alt_d, 6) : "N/A";
  String sats = gps.gps.satellites.isValid() ? String(gps.gps.satellites.value()) : "N/A";

  // Monta uma linha única com os dados para o SD
  String linhaCSV = String(temp, 1) + "," + String(hum, 1) + "," + String(press, 1) + "," +
                    String(uv, 2) + "," + String(co2_v, 2) + "," + String(vbatt, 2) + "," +
                    lat + "," + lon + "," + alt + "," + sats + "," +
                    String(ax, 1) + "," + String(ay, 1) + "," + String(az, 1) + "," +
                    String(gx, 1) + "," + String(gy, 1) + "," + String(gz, 1);

  sd.logLine(linhaCSV);

String resumo =
  "T:" + String(temp, 1) +
  " H:" + String(hum, 1) +
  " P:" + String(press, 1) +
  " V:" + String(vbatt, 2) +
  " CO2:" + String(co2_v, 2) +
  " UV:" + String(uv, 2) +
  " AX:" + String(ax, 1) +
  " AY:" + String(ay, 1) +
  " AZ:" + String(az, 1) +
  " GX:" + String(gx, 1) +
  " GY:" + String(gy, 1) +
  " GZ:" + String(gz, 1) +
  " LAT:" + (gps.gps.location.isValid() ? String(gps.gps.location.lat(), 4) : "N/A") +
  " LON:" + (gps.gps.location.isValid() ? String(gps.gps.location.lng(), 4) : "N/A") +
  " ALT:" + (gps.gps.location.isValid() ? String(gps.gps.altitude.meters(), 4) : "N/A");


  Serial.println(resumo);

  if (lora.isReady() && mysteriousIntType == 0) {
    byte msg[32];                                           //Create the string where the message will be writen
    CreateLoRaMessage(msgId++,lat_d, lon_d, alt_d, vbatt, temp, press, co2_v, uv, msg); //Creates the message to be sent in JSON format
                                      //Sends the mensage via LoRa Radio
    //lora.sendResumo(msg);
  }

  lora.handleRx();
  mysteriousIntType++;
  if(mysteriousIntType == 15)
    mysteriousIntType = 0;
  delay(2000);
}

void CreateLoRaMessage(uint16_t lMsgId, double lat, double lon, double alt, float vbatt, float temp, float press, float co2_v, float uv, byte *query){
  storeUInt16(lMsgId, query, 0);
  lMsgId++;
  storeUInt(millis(), query, 2);               // Same as the SD function
  storeFloat((float) lat, query, 6);
  storeFloat((float) lon, query, 10);
  storeUInt16((uint16_t) alt, query, 14);
  char batata = 8;
  query[16] = (uint8_t) batata; 
  query[17] = (uint8_t) batata;
  query[18] = (uint8_t) batata;
  query[19] = (int8_t) ((vbatt - 3.7) /0.5 * 100);
  query[20] = (int8_t) temp;
  storeFloat(press, query, 21);
  storeUInt16((uint16_t) (co2_v * 100), query, 25);
  storeUInt((uint) (uv * 100), query, 27);

  //8bit checksum
  query[31] = 0;
  for(int i=0; i<31; i++)
    query[31] += query[i];

}


void storeUInt16(uint16_t a, byte *buff, int pointer){
  buff[pointer] = a;
  buff[pointer + 1] = a >> 8;
}

void storeInt16(int16_t a, byte *buff, int pointer){
  buff[pointer] = a;
  buff[pointer + 1] = a >> 8;
}

void storeUInt(uint32_t a, byte *buff, int pointer){
  buff[pointer] = a;
  buff[pointer + 1] = a >> 8;
  buff[pointer + 2] = a >> 16;
  buff[pointer + 3] = a >> 24;
}

void storeFloat(float a, byte *buff, int pointer){
    buff[pointer] = ((uint8_t*)&a)[0];
    buff[pointer + 1] = ((uint8_t*)&a)[1];
    buff[pointer + 2] = ((uint8_t*)&a)[2];
    buff[pointer + 3] = ((uint8_t*)&a)[3];
}

void storeDouble(double a, byte *buff, int pointer){
    buff[pointer] = ((uint8_t*)&a)[0];
    buff[pointer + 1] = ((uint8_t*)&a)[1];
    buff[pointer + 2] = ((uint8_t*)&a)[2];
    buff[pointer + 3] = ((uint8_t*)&a)[3];
    buff[pointer + 4] = ((uint8_t*)&a)[4];
    buff[pointer + 5] = ((uint8_t*)&a)[5];
    buff[pointer + 6] = ((uint8_t*)&a)[6];
    buff[pointer + 7] = ((uint8_t*)&a)[7];
}
