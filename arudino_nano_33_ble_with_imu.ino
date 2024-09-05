#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

BLEService DeviceInformation("183B");
BLECharacteristic Analog("2A58", BLERead | BLEWrite, 20); 

//가속도 및 각도를 저장하기 위한 변수 
float acc_x, acc_y, acc_z;
float acc_pitc, acc_roll;
short acc_pit, acc_rol;
//시간 간격을위한 변수
long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  if(!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while(1);
  }

  BLE.setLocalName("Accelerate");
  BLE.setAdvertisedService(DeviceInformation);
  DeviceInformation.addCharacteristic(Analog);
  BLE.addService(DeviceInformation);
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");

}
void updateAngle() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(acc_x, acc_y, acc_z);
    //각도 계산(라디안을 각도로 180/PI = 57.27755)
    acc_pitc = RAD_TO_DEG * atan(acc_x / sqrt(acc_y *acc_y + acc_z *acc_z));
    acc_roll = RAD_TO_DEG * atan(acc_y / sqrt(acc_x *acc_x + acc_z *acc_z));
    
    acc_pit = acc_pitc;
    acc_rol = acc_roll;

    float data[5] = {acc_x, acc_y, acc_z, acc_pit, acc_rol};
    Analog.writeValue(data, 20);


    //x,y,z 
    Serial.print("x=");
    Serial.print(acc_x);
    Serial.print(", y=");
    Serial.print(acc_y);
    Serial.print(", z=");
    Serial.print(acc_z);

    //pitch, roll 값 
    Serial.print(", pit = ");
    Serial.print(acc_pit);
    Serial.print(", rol = ");
    Serial.println(acc_rol);
  }
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      long currentMillis = millis();
      //50ms마다 보드로 부터 각도 및 위치값 읽어오기
      if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        updateAngle();
      }
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
