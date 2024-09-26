#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

// Definições para o WS2812
#define LED_PIN    21  // Pino onde o WS2812 está conectado
#define NUMPIXELS  1   // Número de LEDs WS2812

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Credenciais da rede Wi-Fi
const char* ssid = "ALTERAR-REDE";
const char* password = "ALTERAR-SENHA";

// Cria o servidor web na porta 80
WebServer server(80);

// Função para configurar o LED WS2812
void setupPixels() {
  pixels.begin();  // Inicializa o NeoPixel
  pixels.show();   // Apaga o LED
}

// Função para atualizar a cor do LED
void updateLED(int r, int g, int b) {
  pixels.setPixelColor(0, pixels.Color(r, g, b));  // Define a cor RGB
  pixels.show();  // Atualiza o LED para mostrar a nova cor
}

// Função que será chamada quando o servidor web for acessado
void handleRoot() {
  String html = "<html><head><title>Controle RGB WS2812</title>";
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; background-color: #1E3A5F; color: white; }";  // Define o fundo azul
  html += "h1 { color: #FFD700; margin-bottom: 5px; }";  // Título principal dourado
  html += "h2 { color: black; margin-top: 0px; }";  // Nome da aplicação em preto
  html += "input[type=range] { width: 300px; height: 30px; }";  // Aumenta o tamanho dos sliders
  html += ".slider-red { background-color: red; }";
  html += ".slider-green { background-color: green; }";
  html += ".slider-blue { background-color: blue; }";
  html += "#colorDisplay { width: 100px; height: 100px; border-radius: 50%; margin: 20px auto; background-color: rgb(0,0,0); }";
  html += "</style>";
  html += "<script>";
  html += "function updateColor() {";
  html += "  var r = document.getElementById('r').value;";
  html += "  var g = document.getElementById('g').value;";
  html += "  var b = document.getElementById('b').value;";
  html += "  document.getElementById('colorDisplay').style.backgroundColor = 'rgb(' + r + ',' + g + ',' + b + ')';";
  html += "  document.getElementById('r').style.background = 'rgb(' + r + ',0,0)';";  // Atualiza cor do slider vermelho
  html += "  document.getElementById('g').style.background = 'rgb(0,' + g + ',0)';";  // Atualiza cor do slider verde
  html += "  document.getElementById('b').style.background = 'rgb(0,0,' + b + ')';";  // Atualiza cor do slider azul
  html += "}";
  html += "</script></head>";
  html += "<body onload='updateColor()'>";
  
  // Título principal
  html += "<h1>LabMaker.Tech</h1>";  // Título dourado
  html += "<h2>Controle RGB WS2812</h2>";  // Subtítulo em preto
  
  // Sliders
  html += "<form action=\"/setRGB\" oninput='updateColor()'>";
  html += "R: <input type=\"range\" class=\"slider-red\" id=\"r\" name=\"r\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "G: <input type=\"range\" class=\"slider-green\" id=\"g\" name=\"g\" min=\"0\" max=\"255\" value=\"0\"><br>";
  html += "B: <input type=\"range\" class=\"slider-blue\" id=\"b\" name=\"b\" min=\"0\" max=\"255\" value=\"0\"><br>";
  
  // Círculo para mostrar a cor
  html += "<div id='colorDisplay'></div>";
  
  html += "<input type=\"submit\" value=\"Atualizar\">";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

// Função para ler os valores RGB do formulário e atualizar o LED
void handleSetRGB() {
  int r = server.arg("r").toInt();
  int g = server.arg("g").toInt();
  int b = server.arg("b").toInt();
  
  updateLED(r, g, b);
  
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  setupPixels();

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Configura as rotas do servidor web
  server.on("/", handleRoot);
  server.on("/setRGB", handleSetRGB);

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();  // Processa as requisições do cliente
}
