#include <WiFi.h> //Biblioteca para o módulo WiFi

//Pino do Led
const int pinoLed = 25; 

//Nome e senha da rede Wi-Fi simulada 
const char* ssid = "Rede"; //* ponteiro de memória. 
const char* password = "123456789";

void setup(){
  //Configurar o Serial Monitor (Monitor interno)
  Serial.begin(115200);
  delay(1000);//para ganhar um tempo

  //Configurar o pinoLed como saída 
  pinMode(pinoLed, OUTPUT);// o que eu quero e como ele irá se comportar, ou seja, uma saída

  //Começar o ESP32 ao WiFi
  Serial.print("Conectando-se à rede Wi-Fi...");
  WiFi.begin(ssid, password);//Inicia a conexão na rede 
  //Aguarda até o status de conexão ser igual
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Enquanto o Wifi.status for diferente de conectado, ele vai colocando o . , preenchendo a barrinha. 
  //Do cotrário e ele conectar, exibir:
  Serial.println("/n Conectado ao Wi-Fi com sucesso!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //para pegar o IP do WiFi
}

void loop(){
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite (pinoLed, HIGH);
  }else{
    digitalWrite(pinoLed, LOW);
  }
}
//Se o wifi conectar, pegar o pino e ligar 
//e se estiver desconectado, quero que apague