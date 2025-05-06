#include <DHT.h>
#include <DHT_U.h>

#include <Adafruit_Sensor.h>

#include <U8g2lib.h>
#include <U8x8lib.h>

#define DHTTYPE DHT11
#define DHTPIN 23

#define SUMA 100
#define RESTA 200
#define P1 300
#define P2 400
#define ESPERA1 500
#define ESPERA2 600
#define BOTON1 35
#define BOTON2 34

int Estado = P1;
bool Pantalla;
int Umbral = 28;
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
DHT dht(DHTPIN, DHTTYPE);
void maquina_de_estados(float temperatura, int EstadoBoton1, int EstadoBoton2);
int boton1;
int boton2;
void setup() {
  Serial.begin(9600);
  Serial.println(F("OLED test"));
  u8g2.begin();
  dht.begin();
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  pinMode(25, OUTPUT);
}

void loop() {
  boton1 = digitalRead(BOTON1);
  boton2 = digitalRead(BOTON2);
  float temperatura = dht.readTemperature();
  maquina_de_estados(temperatura, boton1, boton2);
  delay(100);
}

void maquina_de_estados(float temperatura, int EstadoBoton1, int EstadoBoton2) {
  char stemperatura[6];
  char sUmbral[5];
  switch (Estado) {
    case P1:
      u8g2.clearBuffer();
      sprintf(stemperatura, "%.2f", temperatura);
      sprintf(sUmbral, "%d", Umbral);
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(15, 15, stemperatura);
      u8g2.drawStr(15, 30, sUmbral);
      u8g2.sendBuffer();
      if (temperatura > Umbral) {
        digitalWrite(25, HIGH);
      }
      else{
        digitalWrite(25, LOW);
      }
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        Estado = ESPERA1;
      }
      break;
    case ESPERA1:
    if (EstadoBoton1 == HIGH && EstadoBoton2 == HIGH) {
    Estado = P2;
    }
    break;
    case ESPERA2:
    if (EstadoBoton1 == HIGH && EstadoBoton2 == HIGH) {
    Estado = P1;
    }
    break;
    case P2:
      u8g2.clearBuffer();
      sprintf(sUmbral, "%d", Umbral);
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(15, 30, sUmbral);
      u8g2.sendBuffer();
      if (EstadoBoton1 == LOW && EstadoBoton2 == LOW) {
        Estado = ESPERA2;
      }
      if (temperatura > Umbral) {
        digitalWrite(25, HIGH);
      }
      else{
        digitalWrite(25, LOW);
      }
      if (EstadoBoton1 == LOW && EstadoBoton2==HIGH) {
        Estado = SUMA;
      }
      if (EstadoBoton2 == LOW && EstadoBoton1==HIGH) {
        Estado = RESTA;
      }
      break;
    case SUMA:
      Umbral = Umbral + 1;
      Estado = P2;
      break;
    case RESTA:
      Umbral = Umbral - 1;
      Estado = P2;
      break;
  }
}
