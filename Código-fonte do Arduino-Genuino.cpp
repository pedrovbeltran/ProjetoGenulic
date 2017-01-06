#include <SoftwareSerial.h>

#define DEBUG true

#define Pecho 6
#define Ptrig 7
#define METADE 12
#define CHEIA 13
#define DEL 11
long duracao, distancia; 

SoftwareSerial esp8266(2,3); // Pino 2 RX, Pino 3 TX
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(115200); // velocidade de transmissão padrão do ESP8266

  sendData("AT+RST\r\n",2000,DEBUG); // reseta o módulo
  
  sendData("AT+CIOBAUD=19200\r\n",1000,DEBUG); // muda a velocidade para 19200, já que a SoftwareSerial.h trabalha com 19200
  esp8266.begin(19200);

  //****************************** Configuração dos pinos de entrada e saída ************************************************
  
  pinMode(CHEIA,OUTPUT);
  digitalWrite(CHEIA,LOW);
  
  pinMode(METADE,OUTPUT);
  digitalWrite(METADE,LOW);

  pinMode(DEL,OUTPUT);
  digitalWrite(DEL,LOW);

  pinMode(Pecho, INPUT);     // define o pino 6 como entrada (echo)
  pinMode(Ptrig, OUTPUT);    // define o pino 7 como saída  (trigger)
  
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configura o módulo como estação
  sendData("AT+CIFSR\r\n",1000,DEBUG); // este comando nos indica qual a configuração de IP do módulo
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // Configura várias conexões
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // Inicializa o web server
}
void loop()
{
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // gera o pulso do trigger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);
  
  duracao = pulseIn(Pecho, HIGH);
  distancia = (duracion/2) / 29;            // calcula a distância em centímetros
    
  if (distancia <= 15)
  {
    digitalWrite(CHEIA, LOW);
    digitalWrite(METADE, LOW);
  }
  else if (distancia > 15 && distancia <= 25)
  {
    digitalWrite(CHEIA,LOW);
    digitalWrite(METADE,HIGH);
  }
  else
  {
    digitalWrite(CHEIA,HIGH);
    digitalWrite(METADE,HIGH);
  }
  delay(500);

///******************************************** Parte para o módulo Wi-fi ***************************************************

  if(esp8266.available()) // Verifica se o módulo está disponível
  { 
    if(esp8266.find("+IPD,")) //Procura o campo de texto +IPD
    {
       delay(1000); //Espera um segundo para ler todos os dados da porta serial
  
       int connectionId = esp8266.read()-48;
  
       if(esp8266.find("lamp"))
       {
           if(esp8266.find("+des"))
           { 
             digitalWrite(DEL, LOW);
           }  
           else
           { 
             digitalWrite(DEL, HIGH);
           }
       }
       else
       {
          String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
          webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>LED: ";
          //webpage += " Azul: ";
          int a = digitalRead(DEL);
          webpage += a;
          webpage += "<h2>Caixa d'água: ";
          webpage += distancia;
          webpage += " cm";
          
          webpage += "</h2>";
     
          String cipSend = "AT+CIPSEND=";
          cipSend += connectionId;
          cipSend += ",";
          cipSend += webpage.length();
          cipSend += "\r\n";
     
          sendData(cipSend, 1000, DEBUG);
          sendData(webpage, 1000, DEBUG);
       }
  
       String closeCommand = "AT+CIPCLOSE="; 
       closeCommand+=connectionId; 
       closeCommand+="\r\n";
       
       sendData(closeCommand,3000,DEBUG); // encerra a conexão
    }
  }
}


String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}
