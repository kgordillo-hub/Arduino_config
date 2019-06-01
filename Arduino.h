#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* Connection Pins
Arduino    Microphone
 GND          GND
 +5V          +5V
 D3           OUT (or D0) depends on Microphone
*/

#define ADC 0  // analog 0

//const int ADC; // define D0 Sensor Interface verificar****
float val;// define numeric variables val
int cont = 1;
float valSend = 0.0;
const String domainRestApi = "https://53orribw49.execute-api.us-east-2.amazonaws.com/"; //Domain restApi
String restNoise = "http://ws-medical-device.azurewebsites.net/medical/sendSignal"; //Endpoint noise
//String restConfigNoise = domainRestApi + "api/NoiseConfig"; //Endpoint noiseConfig

void setup ()
{ 
  Serial.begin(9600);
  //interruptSetup();
 
  WiFi.begin("user", "password");

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(400);
    Serial.println("Waiting for connection");
    Serial.println();


  }
  
  //pinMode (ADC, INPUT) ;// output interface D0 is defined sensor//
}

void loop ()
{
  //delay(1);
  cont = cont - 1;
  val = analogRead(ADC);// digital interface will be assigned a value of pin 3 to read val

  //if(val > 0){
    //val = 10*log(val);
  //}

  if(valSend < val){
    valSend = val;
   }

  if(cont == 0){
    cont = 1;
    Serial.println(valSend);
    Serial.println();
    String strDecibeles = (String) valSend;//aca va valSend
    
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      Serial.println(restNoise);
      http.begin(restNoise);  
      http.addHeader("Content-Type", "application/json"); 
      String requestJson = "{\"dato\":" + strDecibeles + "}";
      Serial.println(requestJson);
      int httpCode = http.POST(requestJson);
      Serial.println(httpCode + "este es el json ......");
      Serial.println("valor cardiaco: " + strDecibeles);
      String strHttpCode = (String) httpCode;
      Serial.println("Respuesta del web service: " + strHttpCode);
      Serial.println();
      Serial.println();
      Serial.println();
      //Serial.println();
      //Serial.println();

      
    } else {
      Serial.println("No se pudo enviar al api");
    }
    
    valSend = 0;
  }
}
