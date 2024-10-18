#define HAS_WIFI 1
#define HAS_BLUETOOTH 1
#define HAS_GPS 0
#define MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR 1


// ST7789 TFT LCD
#define HAS_SCREEN 1
#define USE_SH1106SPI 1
#define OLED_MOSI 11
#define OLED_SCLK 10
#define OLED_CS 14
#define OLED_DC 13
#define OLED_RST 12
#define OLED_SPI_HOST SPI3_HOST

// #define D5  OLED_SCLK
// #define D7  OLED_MOSI
// #define D0  OLED_RES
// #define D2  OLED_DC
// #define D8  OLED_CS
// #define USE_SH1106

#define MESHTASTIC_EXCLUDE_I2C 1
#define I2C_SCL -1
#define I2C_SDA -1

#define SLEEP_TIME 120

#define BUTTON_PIN 0
// #define BUTTON_NEED_PULLUP

// #define GPS_RX_PIN 44
// #define GPS_TX_PIN 43

#define BATTERY_PIN 4 // A battery voltage measurement pin, voltage divider connected here to measure battery voltage
// ratio of voltage divider = 2.0 (RD2=100k, RD3=100k)
#define ADC_MULTIPLIER 2.11 // 2.0 + 10% for correction of display undervoltage.
#define ADC_CHANNEL ADC1_GPIO4_CHANNEL

// keyboard
// #define I2C_SDA 18 // I2C pins for this board
// #define I2C_SCL 8
// #define CANNED_MESSAGE_MODULE_ENABLE 0

// encoder
// TODO: add encoder support


// microphone
// #define ES7210_SCK 47
// #define ES7210_DIN 14
// #define ES7210_LRCK 21
// #define ES7210_MCLK 48

// dac / amp
// #define HAS_I2S
// #define DAC_I2S_BCK 7
// #define DAC_I2S_WS 5
// #define DAC_I2S_DOUT 6

// LoRa
#define USE_SX1262
#define USE_SX1268

#define LORA_SCK 41
#define LORA_MISO 42
#define LORA_MOSI 45
#define LORA_CS 46

#define LORA_DIO0 15 // a No connect on the SX1262 module
#define LORA_RESET 9
#define LORA_DIO1 -1 // SX1262 IRQ
#define LORA_DIO2 -1 // SX1262 BUSY
#define LORA_DIO3  -1  // Not connected on PCB, but internally on the TTGO SX1262, if DIO3 is high the TXCO is enabled

#define SX126X_CS LORA_CS // FIXME - we really should define LORA_CS instead
#define SX126X_DIO1 LORA_DIO1
#define SX126X_BUSY LORA_DIO2
#define SX126X_RESET LORA_RESET
// Not really an E22 but TTGO seems to be trying to clone that
#define SX126X_DIO2_AS_RF_SWITCH
#define SX126X_DIO3_TCXO_VOLTAGE 1.8
// Internally the TTGO module hooks the SX1262-DIO2 in to control the TX/RX switch (which is the default for the sx1262interface
// code)