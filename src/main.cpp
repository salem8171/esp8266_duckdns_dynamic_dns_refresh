// http://www.duckdns.org/update/mskhome/3acfbe58-480e-44e8-935f-d24ad02f0715

/*
    HTTP over TLS (HTTPS) example sketch
    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.
    Limitations:
        only RSA certificates
        no support of Perfect Forward Secrecy (PFS)
        TLSv1.2 is supported since version 2.4.0-rc1
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#ifndef STASSID
#define STASSID "ORANGE_B4F0"
#define STAPSK  "NXCJG3P6"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "www.duckdns.org";
// const int httpsPort = 443;
const int httpPort = 80;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
// const char fingerprint[] PROGMEM = "D5 64 5C 51 2B 02 4D 4B 49 67 8F 91 09 D3 CA 0F 72 AD 60 E2";

void setup()
{
    Serial.begin(115200);
    delay(10);
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    // Use WiFiClientSecure class to create TLS connection
    //   WiFiClientSecure client;
    WiFiClient client;
    Serial.print("connecting to ");
    Serial.println(host);

    //   Serial.printf("Using fingerprint '%s'\n", fingerprint);
    //   client.setFingerprint(fingerprint);

    //   if (!client.connect(host, httpsPort)) {
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        delay(2000);
        return;
    }

    // String url = "/update/mskhome/3acfbe58-480e-44e8-935f-d24ad02f0715";
    String url = "/update?domains=mskhome&token=3acfbe58-480e-44e8-935f-d24ad02f0715";
    
    Serial.print("requesting URL: ");
    Serial.println(url);

    //   client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    //                "Host: " + host + "\r\n" +
    //                "User-Agent: BuildFailureDetectorESP8266\r\n" +
    //                "Connection: close\r\n\r\n");
                
    client.print(String("GET ") + url + " HTTP/1.1\r\n"
                + "Host: " + host + "\r\n"
                + "User-Agent: BuildFailureDetectorESP8266\r\n"
                + "Connection: close\r\n\r\n"
                + "Accept: */*"
                );

    Serial.println("request sent");

    // while (client.available()) 
    // {
    //     char ch = static_cast<char>(client.read());
    //     Serial.print(ch);
    // }

    // while (client.connected())
    // {
    //     String line = client.readStringUntil('\n');
    //     if (line == "\r") 
    //     {
    //         Serial.println("headers received");
    //         break;
    //     }
    // }
    unsigned long timeout = millis();
    while (client.connected())
    {
        if (millis() - timeout > 5000) break;
        String line = client.readStringUntil('\n');
        Serial.println(line);
    }

    // String line = client.readStringUntil('\n');

    // if (line.startsWith("{\"state\":\"success\""))
    // {
    //     Serial.println("esp8266/Arduino CI successfull!");
    // } else
    // {
    //     Serial.println("esp8266/Arduino CI has failed");
    // }

    // Serial.println("reply was:");
    // Serial.println("==========");
    // Serial.println(line);
    // Serial.println("==========");
    Serial.println("closing connection");
    // client.stop();
    delay(30000);
}