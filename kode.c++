#define BLYNK_TEMPLATE_ID "TMPL6Zk72yZdN"
#define BLYNK_TEMPLATE_NAME "Smart Plant Watering System"

char auth[] = "398DiYFUAH4Fc-YzKRg-25ehS3usv8H2"; // Gantilah dengan token Blynk Anda

#include <BlynkSimpleEsp8266.h>

// char auth[] = "398DiYFUAH4Fc-YzKRg-25ehS3usv8H2"; // Gantilah dengan token Blynk Anda
char ssid[] = "POCO";
char pass[] = "111111111";

BlynkTimer timer;
#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN D0

bool pumpStatus = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  
  pinMode(RELAY_PIN, OUTPUT);
  turnOffPump(); // Pastikan pompa pada awalnya dimatikan
  
  timer.setInterval(10000, checkSoilMoisture); // Periksa kelembaban tanah setiap menit
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkSoilMoisture() {
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  float moisture_percentage = (100.00 - ((analogRead(SOIL_MOISTURE_PIN) / 1023.00) * 100.00));
  Serial.print("Soil Moisture: ");
  Serial.print(moisture_percentage);
  Serial.println("%");
  
  Blynk.virtualWrite(V0, moisture_percentage); // Kirim data kelembaban tanah ke Blynk
  
  if (moisture_percentage < 30 && !pumpStatus) {
    turnOnPump(); // Jika kelembaban tanah kurang dari 30% dan pompa dimatikan, nyalakan pompa
  } else if (moisture_percentage >= 30 && pumpStatus) {
    turnOffPump(); // Jika kelembaban tanah 30% atau lebih dan pompa dinyalakan, matikan pompa
  }
}

void turnOnPump() {
  digitalWrite(RELAY_PIN, LOW); // Matikan relay untuk mematikan pompa
  Serial.println("Pump turned OFF");
  Blynk.virtualWrite(V1, 0); // Perbarui aplikasi Blynk untuk menampilkan status pompa
  pumpStatus = true;
}

void turnOffPump() {
  digitalWrite(RELAY_PIN, HIGH); // Aktifkan relay untuk menghidupkan pompa
  Serial.println("Pump turned ON");
  Blynk.virtualWrite(V1, 100); // Perbarui aplikasi Blynk untuk menampilkan status pompa
  pumpStatus = false;
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    turnOnPump(); // Jika tombol diatur ke nilai 1, hidupkan pompa
  } else {
    turnOffPump(); // Jika tombol diatur ke nilai selain 1, matikan pompa
  }
}