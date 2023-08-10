#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>    //https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
LiquidCrystal_I2C lcd(0x27, 16, 2);


const char* ssid     = "Provider Internet";
const char* password = "kangmus20";

const char* host     = "http://smartandon.000webhostapp.com/";
const char* url      = "http://smartandon.000webhostapp.com/add/add.php";

IPAddress local_IP(192, 168, 43, 200);
IPAddress gateway(192, 168, 43, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
WiFiServer server(80);

int pinTurb = 36;
float V;
float kekeruhan;
float VRata2;
double VHasil;
String data;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  Serial.println("SENSOR KEKERUHAN AIR");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
  data = "";
  server.begin();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("SISTEM  ON");
  delay(2000);
  lcd.clear();
}

void loop()
{
  V = 0;
  for (int i = 0; i < 800; i++)
  {
    V += ((float)analogRead(pinTurb));

    VRata2 = V / 800;
    VHasil = roundf(VRata2 * 10.0f) / 10.0f;
    VHasil = map(VHasil, 0, 4095, 0, 1023);
    VHasil = (VHasil / 1023) * 5;
    VHasil = mapfloat(VHasil, 0, 5, 0.00, 5.00);
  }

  if (VHasil > 4.20)
  {
    kekeruhan = 0;
  }
  else
  {
    //    kekeruhan = ((-37.493 * pow(VHasil,3) + 513.12 * sq(VHasil) - 2615.4 * VHasil + 5965.5) * VHasil) - 4986.4;
    kekeruhan = -120.04 * VHasil + 502.16;
  }

  Serial.print("tegangan :");
  Serial.print(VHasil);
  Serial.print(" V");

  Serial.print("\t kekeruhan :");
  Serial.println(kekeruhan);
  delay(10);
  lcd.setCursor(0, 0);
  lcd.print("Turb: ");
  lcd.print(kekeruhan);
  lcd.print(" NTU");
  if (kekeruhan > 25) {
    lcd.setCursor(0, 1);
    lcd.print("Kondisi: Buruk");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Kondisi: Baik ");
  }
  kirim();
  delay(5000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
