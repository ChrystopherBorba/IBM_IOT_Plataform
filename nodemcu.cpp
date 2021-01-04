#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<DHT.h>

#define DHTTYPE DHT11

const char* nome_wifi = "usuário";
const char* senha_wifi = "senha";

const char* broker = "ID da organização.messaging.internetofthings.ibmcloud.com";
const char* usuario_mqtt="use-token-auth";
const char* senha_mqtt = "Token de autenticação";
const char* cliente_mqtt = "d:ID da organização:Tipo de dispositivo:ID do dispositivo";
const char* topico = "iot-2/evt/data/fmt/json";
const int porta_mqtt = 1883;


int pino_DHT = 2;
DHT dht(pino_DHT, DHTTYPE);

WiFiClient wificlient;
PubSubClient client(broker, porta_mqtt, NULL, wificlient);




void conectarWifi(){
  Serial.print("Conectando na rede");
  WiFi.begin(nome_wifi, senha_wifi);
  while (WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado no Wifi");
}

void conectarMQTT(){
  Serial.print("Conectando no Broker...");
  client.setServer(broker, porta_mqtt);
  while (!client.connect(cliente_mqtt, usuario_mqtt, senha_mqtt))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Conectado com o broker!");
  
}
void enviar(String nome_campo1, float valor1, String nome_campo2, float valor2){

  if(!client.connected()){
    conectarMQTT();
    
  }
  //{ "d": {"key1": "value1"} }
  String payload = "{\"d\":{\""+ nome_campo1 + "\":" + valor1 + ", \""+ nome_campo2 + "\":" + valor2 + " } }";
  client.publish(topico, (char*) payload.c_str());
}

void setup() {
  Serial.begin(115200);


  pinMode(pino_DHT, INPUT);

  conectarWifi();
  conectarMQTT();

}
void loop() {

  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  if((!isnan(temp)) || (!isnan(humi))){
    enviar("Temperatura", temp, "Umidade", humi);
    Serial.print("temperatura:");
    Serial.println(temp);
    Serial.print("Umidade:");
    Serial.println(humi);
  }
  else{
    Serial.println("Ruído");
  }
  delay(1000);
}




