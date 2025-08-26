#include <WiFi.h> //biblioteca para conexão WiFi
#include <PubSubClient.h> //biblioteca para conexão MQTT
#include <DHT.h> //biblioteca para módulo DHT11/DHT22

// configurações para rede Wi-Fi 
const char* ssid = "Rede";
const char* password = "123456789";


// configuração do Broker MQTT
const char* mqttServer = "broker.hivemq.com"; // Endereço do broker publico 
const int mqttPort = 1883; // porta padrão MQTT
const char* mqttTopic = "caputti/lerSensor"; // Tópico onde os dados serão publicados

//Configuração do DHT22
#define DHTPIN 21 // Pino onde o sensor DHT esá conectado
#define DHTTYPE DHT11 // tipo do sensor
DHT dht(DHTPIN, DHTTYPE); //Cria objeyo "dht" com o pino e tipo do sensor

bool wasConnected = false; // Variavel de status de conexão 

// Instancia dos Objetos das Bibliotecas 
WiFiClient espClient; // Cliente do WiFi
PubSubClient client ( espClient ); // Cliente MQTT usando WiFi 

unsigned long lastMsg = 0; //Armazenar o tempo da ultima mensagem enviada
const long interval = 2000; // Intervalo entre publicações (2 segundos)

const int pinoLed = 2; // Pino do LED embutido no ESP32

void setup_wifi() {
  delay(10);  // Pequeno atraso inicial

// Configurar o pinoLed como saída
  pinMode(pinoLed, OUTPUT);  
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password); // Inicia conexão Wi-Fi
  while (WiFi.status() != WL_CONNECTED) { // Aguarda até conectar
    delay(500); Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado. IP: " + WiFi.localIP().toString());
}

// Função para reconectar ao Broker MQTT (Servidor), se desconectado.
void reconnect(){
  // ID Randomico
  String clientId = "Esp32_" + String(random(0xffff), HEX);
  while(!client.connected()){
    Serial.println("Tentando conectar ao MQTT...");
    if(client.connect(clientId.c_str())){ // Tenta fazer a conexao com o ID Esp32Client 
      Serial.println("Conectado ao Broker.");
    }else{
      Serial.println("Falha ao conectar-se... Tentando novamente em 5s...");
      Serial.print("Código da Falha: ");
      Serial.println(client.state()); // Exibe codigo do erro
      delay(5000);  // Espera para tentar novamente
    }
  }
}

void setup(){
  Serial.begin(115200); // inicia comunicação serial
  dht.begin();  // inicializa o sensor DHT22
  setup_wifi(); // inicia e conectano wifi 
  client.setServer(mqttServer, mqttPort); // define o servidor MQTT e porta
}

void loop(){
   if(WiFi.status() == WL_CONNECTED){
      digitalWrite(pinoLed, HIGH);
    }else{
      digitalWrite(pinoLed, LOW);
    }
  if(!client.connected()){ //verifca se está conectado ao broker
    reconnect(); //se não estiver, tenta reconectar
  }
  client.loop(); // mantém a conexão MQTT ativa

  unsigned long now = millis(); //Pega o tempo atual
  if(now - lastMsg > interval ){ //Verifica se já passou o intervelo de envio
    lastMsg = now; //Atualiza o tempo da última mensagem

    //Pegar os valores de temperatura e umidade do sensor
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if(isnan(h) || isnan(t)){ // Verifica se houve erro na leitura
        Serial.println("Erro ao ler Sensor DHT11.");
        return; // Sai da função se houver erro 
    }

    //Cria uma variavel no formato JSON]
    String payload = "{";
    payload += "\"temperatura\": " + String(t,1) + ","; // Um dígito decimal
    payload += "\"umidade\": " + String(h,1); // Um dígito decimal
    payload += "}";

    Serial.print("Publicando: ");
    Serial.println(payload);
    client.publish(mqttTopic, payload.c_str()); // Publica no tópico definido


  }

}