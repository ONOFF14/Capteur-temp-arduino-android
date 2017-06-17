// Bluetooth low energy (nRF8001) + capteur de température TMP36 
#include <SPI.h>
#include <BLEPeripheral.h>

BLEPeripheral blePeripheral = BLEPeripheral(10,2,9);
BLEService thermometerService("BBB0");
BLEFloatCharacteristic temperatureCharacteristic("BBB1", BLERead | BLENotify);
BLEDescriptor temperatureDescriptor("2901", "degrees C");

#define TEMPERATURE_PIN A0

long previousMillis = 0;  
long interval = 2000;     
void setup()
{
  Serial.begin(9600);
  Serial.println(F("Bluetooth Low Energy Thermometer"));

  blePeripheral.setLocalName("Thermometer");
  blePeripheral.setDeviceName("Thermometer");
  blePeripheral.setAdvertisedServiceUuid(thermometerService.uuid());

  blePeripheral.addAttribute(thermometerService);
  blePeripheral.addAttribute(temperatureCharacteristic);
  blePeripheral.addAttribute(temperatureDescriptor);

  blePeripheral.begin();
}

void loop()
{
  blePeripheral.poll();

   if(millis() - previousMillis > interval) {
    pollTemperatureSensor();
    previousMillis = millis();
  }
}

void pollTemperatureSensor()
{
  float temperature = calculateTemperature();

  if (temperatureCharacteristic.value() != temperature) {
    temperatureCharacteristic.setValue(temperature);
    Serial.println(temperature);
  }
}

  float calculateTemperature()
{
  int sensorValue = analogRead(TEMPERATURE_PIN);

  float voltage = sensorValue * 5.0;
  voltage /= 1024.0;
  float temperature = (voltage - 0.5) * 100; 
  return temperature;
}
