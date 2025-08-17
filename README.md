Resumo do funcionamento:
O programa inicializa sensores e módulos (GPS, bateria, sensores ambientais, SD, LoRa, display OLED), coleta dados ambientais e de navegação, exibe informações no display, salva os dados em um arquivo CSV no cartão SD e prepara mensagens para transmissão via rádio LoRa.

## Configuração da placa Lora32 na IDE do Arduino

1. Adicione o gerenciador de placas Heltec:
	- Vá em: Arquivo → Preferências → URLs adicionais para Gerenciadores de Placas
	- Adicione:
	  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/3.0.2/package_heltec_esp32_index.json

2. Selecione a placa:
	- Ferramentas → Placa → Heltec ESP32 Series Dev-Boards → WiFi Lora 32(V3)


## Bibliotecas necessárias

Baixar as bibliotecas necessárias:

# Instalar sempre com as bibliotecas dependentes sugeridas
- Adafruit_BME280_Library (by Adafruit) (2.3.0)
- Adafruit BusIO (1.17.2)
- Adafruit GFX Library (1.12.1)
- Adafruit LIS3MDL (1.2.4)
- Adafruit LSM9DS1 library (2.2.1)
- Adafruit MPU6050 (2.2.6)
- Adafruit SSD1306 (2.5.15)
- Adafruit Unified Sensor (1.1.15)
- ESP32Servo (by Kevin Harrington) (3.0.8)
- ESP8266 and ESP32 OLED driver for SSD1306 displays (by ThingPulse, Fabrice Weinberg) (4.6.1)
- Heltec ESP32 Dev-Boards (by Heltec Automation) (2.1.4)
	- # essa biblioteca tem uma segunda declaração de TinyGPS e entra em conflito CUIDADO!
- Heltec_ESP32_LoRa_v3 (by Rop Gonggrijp) (0.9.2)
- HotButton (by Rop Gonggriip) (0.1.1)
- MPU9250_asukiaaa (by Asuki Kono) (1.5.13)
- RadioLib (by Jan Gromes) (7.1.2)
- TinyGPSPlus (by Mikal Hart) (1.0.3)
- U8g2 (by oliver) (2.35.30)

