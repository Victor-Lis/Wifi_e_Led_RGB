
# Alterando cores do Led RGB via Wifi

Nesse projeto tive a experiência de pela primeira vez utilizar um Led RGB e também foi minha primeira vez de fato lendo parâmetros http usando C++.

## Desafios

Acredito que meus principais desafios nesse projeto foram:
- Rodar o Esp32 como um servidor.
- Primeira vez usando um Led RGB
- Receber cores rgb via método get na url.
## Aprendizados

Por final aprendi algumas coisas interessantes como: 
### Criando uma server wifi com o Esp32

```c++
#include <iostream>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

// Set these to your desired credentials.
const char *ssid = "esp32_server";
const char *password = "esp32@server";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}
```

## Convertendo tipagem das variáveis

### Int para String
Nesse caso eu passo uma variável do tipo Int como parâmetro entre a String(Int).
```c++
  String red = String(currentLine[33]) + String(currentLine[34]) + String(currentLine[35]);
```

### String para Int
Nesse caso eu pego a variável String "red", declarada no exemplo acima e simplesmente adiciono .toInt(), transformando-a em Int.
```c++
  analogWrite(PIN_RED, red.toInt());
```

### Recebendo valores da web
```c++
#define PIN_RED    23 // GPIO23
#define PIN_GREEN  22 // GPIO22
#define PIN_BLUE   21 // GPIO21

void setup() {
    pinMode(PIN_RED,   OUTPUT); // Inicializando o pino vermelho
    pinMode(PIN_GREEN, OUTPUT); // Inicializando o pino verde
    pinMode(PIN_BLUE,  OUTPUT); // Inicializando o pino blue
}

void loop() {
    ...
    if(...){
        analogWrite(PIN_RED, red.toInt());
        analogWrite(PIN_GREEN, green.toInt());
        analogWrite(PIN_BLUE, blue.toInt());
    }
    ...
}
```

### Recebendo valores via GET
No trecho abaixo pego os indices de cada parâmetro da url (sendo do tipo String) e os converto para int.
Verfiquei se o método continha "Referer", se tivesse, seria a minha url. Em seguida verifiquei se a "currentLine" atual tinha exatamente 44 caractéres. Para ficar mais fácil de entender, a url que ele vai ler é parecida com a seguinte url:
```c++
http://192.168.4.1/?rgb=200100150
(Exatamente 44 caractéres)

Indices Vermelhos: [33, 34, 35]
Indices Verdes: [36, 37, 38]
Indices Azuis: [39, 40, 41]

Resultado (de acordo com o exemplo):
Vermelho: 200
Verde: 100
Azul: 150

``` 
```c++
void loop(){
    if ((currentLine.indexOf("Referer")+1) && currentLine.length() == 42) {
        Serial.println(currentLine);
        String red = String(currentLine[33]) + String(currentLine[34]) + String(currentLine[35]);
        String green = String(currentLine[36]) + String(currentLine[37]) + String(currentLine[38]);

        String blue= String(currentLine[39]) + String(currentLine[40]) + String(currentLine[41]);

        analogWrite(PIN_RED, red.toInt());
        analogWrite(PIN_GREEN, green.toInt());
        analogWrite(PIN_BLUE, blue.toInt());
    }
}
```
# Resultado

[Ver vídeo do projeto funcionando!](https://youtube.com/shorts/0fI4GBiOdVI?feature=share)
## Autores e Pessoas que ensinei durante o Projeto.

- [@Iago-Rodrigues](https://github.com/iagoRRocha)
- [@Karlos Eduardo](https://github.com/ImpressoraTelepatica)
- [@Miguel Rosillo](https://github.com/MiguelRED1209)
- [@Pedro Henrique](https://github.com/PedroHenriqueMoraesSamsonas)
- [@Victor-Lis](https://github.com/Victor-Lis)
