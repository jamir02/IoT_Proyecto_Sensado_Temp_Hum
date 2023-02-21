#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 17
#define DHTTYPE DHT11

#define SAMPLE_WINDOW_LEN 1000

const char *ssid = ""; // Escribe el nombre de tu red
const char *pwd = ""; // Escribe la contraseña de tu red

float temps[60] = {'\0'};
float hums[60] = {'\0'};
uint16_t dataIdx = 0;

uint32_t windowStart;

String serverName = "http://192.168.0.35:5000/sensor/tempHumSensor";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Empezando programa");

  WiFi.begin(ssid, pwd);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red WiFi con direccion IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  windowStart = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis() - windowStart) >= SAMPLE_WINDOW_LEN)
  {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Error al leer el sensor");
      return;
    }

    hums[dataIdx] = h;
    temps[dataIdx] = t;
    dataIdx++;

    Serial.println("Lectura realizada");

    windowStart = millis();
  }

  if(dataIdx == 60)
  {
    float hProm = 0.0;
    float tProm = 0.0;

    for(int i=0; i<dataIdx; i++)
    {
      hProm += hums[i];
      tProm += temps[i];
    }

    hProm /= dataIdx;
    tProm /= dataIdx;

    Serial.print("Humedad promedio: ");
    Serial.print(hProm);
    Serial.print("\tTemperatura promedio: ");
    Serial.println(tProm);

    if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"temp\": " + String(tProm) + ", \"hum\": " + String(hProm) +"}";
    int httpCode = http.POST(payload);
    if(httpCode > 0) {
      Serial.print("Codigo de estado: ");
      Serial.println(httpCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.println("Error en la peticion");
    }
    http.end();
  } else {
    Serial.println("No hay conexion a la red WiFi");
  }

    dataIdx = 0;
  }
}