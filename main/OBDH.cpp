#include "OBDH.h"
#include "Pins.h"
#include <SD.h>
#include <SPI.h>
#include "LoRaWan_APP.h"

SPIClass SPI_SD(HSPI);
File dataFile;
const char *logFilename = "/dados.csv";

int SDCardHandler::begin() {
  SPI_SD.begin(PIN_SD_CLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  if (!SD.begin(PIN_SD_CS, SPI_SD)) {
    Serial.println("[SD] Falha ao iniciar cartão.");
    return -1;
  }

  dataFile = SD.open(logFilename, FILE_APPEND);
  if (!dataFile) {
    Serial.println("[SD] Erro ao abrir arquivo de dados.");
    return -2;
  }

  return 0;
}

String SDCardHandler::timestamp() {
  return String(millis());  // ou RTC no futuro
}

void SDCardHandler::logLine(const String &line) {
  dataFile.println(timestamp() + "," + line);
  dataFile.flush();
}

// ====================== LoRa ======================

#define RF_FREQUENCY                                928000000 // Hz

#define TX_OUTPUT_POWER                             20        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       12         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 200 // Define the payload size here



static RadioEvents_t RadioEvents;
static char rxpacket[BUFFER_SIZE];
static LORA32 *instance;

static void OnTxDone() {
  instance->sending = false;
  Serial.println("[LoRa] TX ok.");
}

static void OnTxTimeout() {
  instance->sending = false;
  Serial.println("[LoRa] Timeout.");
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  memcpy(rxpacket, payload, size);
  rxpacket[size] = '\0';
  instance->lastReceived = String(rxpacket);
  Serial.printf("[LoRa] RX: %s\n", rxpacket);
}

void LORA32::begin() {
  instance = this;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;
  setupLoRa();
  Radio.Rx(0);
}

void LORA32::setupLoRa() {
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,                  // Configures Radio Tx
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );        
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}

void LORA32::sendResumo(byte *linha) {
  sending = true;
  Radio.Send((uint8_t *)linha,32);
}

void LORA32::handleRx() {
  Radio.IrqProcess();
}

String LORA32::getLastCommand() {
  return lastReceived;
}

bool LORA32::isReady() {
  return !sending;
}
