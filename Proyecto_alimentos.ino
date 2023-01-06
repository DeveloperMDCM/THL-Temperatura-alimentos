
#define retardo 3000 //ms
float l = 0;
//Librerias para display I2C y Sensor DHT22
#include "ThingSpeak.h"
#include "WiFi.h"
#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <DHT.h> //Libreria para el sensor digital DHT22
#include <DHT_U.h>
#define DHTPIN 33 //pin 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT22
// Inicializamos el sensor DHT12
const char* ssid = "CALILLITA";                        //SSID de vuestro router.
const char* password = "MDCM2022";                //Contraseña de vuestro router.

unsigned long channelID = 1749775;                //ID de vuestro canal.
const char* WriteAPIKey = "3XTUA2PDYPZIDWTE";     //Write API Key de vuestro canal.
DHT dht(DHTPIN, DHTTYPE); // declaramos el pin de conexion y el tipo de sensor a utilizar
WiFiClient cliente;



void setup() {
  Serial.begin(9600);   //Inicialisamos el monitor serial
  dht.begin(); // Inicimos el sensor DHT
   mlx.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wifi conectado!");
  ThingSpeak.begin(cliente);
}



//Ciclo de repeticion infinita
void loop()
{
  //delay(1800000);
  delay(1000);
  // Esperamos 1 segundos entre medidas y/o lecturas
  mostrarDatos(); // Funcion que muestra todos los datos
  ThingSpeak.writeFields(channelID, WriteAPIKey);
  Serial.println("Datos enviados a ThingSpeak!");
  delay(500);
}

void mostrarDatos() {  //Se declara la función mostrarDatos(), esta no devuelve ningún valor ni recibe parámetros
   float LDR = analogRead(35);
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  float ob = mlx.readObjectTempC();
  // Comprobamos si esta conectado el sensor o si hay un error en la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al obtener los datos del sensor DHT12");
    return;
  }
  //los Serial.print imprimen mensajes en el monitor serial y los Display.print inprimen en el display IC2 oled
  Serial.print("Humedad: "); //Imprime la palabra humedad
  Serial.print(h); //Imprime el valor de humedad
  Serial.print(" %"); //Imprime %
  Serial.println(""); //Salto de linea
  Serial.print("Temperatura: "); //Imprime la palabrañporcentaje correspondiente en luminocidad
  Serial.print(t);
  Serial.println(""); //Salto de linea
  Serial.print("Luminocidad: ");//Imprime la palabra humedad
  l = (LDR * 100) / 4095.0 ;
  Serial.print(l,2); //Imprime el porcentaje de luminocidad con 2 cifras
  Serial.print(" %");
  Serial.println(""); //Salto de linea
 Serial.print("*C\tObject = "); 
 Serial.print(mlx.readObjectTempC()); 
 Serial.println("*C");
  ThingSpeak.setField (1, t);
  ThingSpeak.setField (2, h);
  ThingSpeak.setField (3, l);
  ThingSpeak.setField (4, ob);


}
