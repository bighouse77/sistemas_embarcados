#include <WiFi.h>

const char* ssid = "LUIZINHO_S2";
const char* password = "12345678";
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
 
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());
 
  server.begin();
}
 
void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<div style='height: auto; width: auto'><h1 style='text-align:center'>Home Control</h1><br><form style='text-align:center'><label for=''><h4>Temperatura</h4></label><input type='number' name='' id=''> C°<br> <br><p><h4>Iluminação</h4></p><input type='radio' id='claro' name='fav_language' value='claro'><label for='claro'>Claro</label><input type='radio' id='blackout' name='fav_language' value='blackout'><label for='blackout'>Blackout</label><input type='radio' id='semiblackout' name='fav_language' value='semiblackout'><label for='semiblackout'>Semi Blackout</label><br><br><input type='submit'></form></div>");
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED, HIGH);
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED, LOW);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}