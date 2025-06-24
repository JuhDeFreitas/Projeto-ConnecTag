#define BLYNK_TEMPLATE_ID "TMPL2ftf0FkDt"
#define BLYNK_TEMPLATE_NAME "ConnecTag"
#define BLYNK_AUTH_TOKEN "nt2AS16SgSOR_bSTNBK-_0D7wz9uBiZY"

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

String uids_validos[] = {
  "774B793C",  
  "EB2CBE1B",
  "11223344"
};

// Wi-Fi
const char* ssid = "WLL-Inatel";
const char* password = "inatelsemfio";
int contador = 0;

// RC522
#define SS_PIN 4
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);

BlynkTimer timer;

bool uidEhValido(String uid) {
  for (int i = 0; i < sizeof(uids_validos) / sizeof(uids_validos[0]); i++) {
    if (uids_validos[i] == uid) {
      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  Serial.println("Aproxime um cartão RFID...");
}

void loop() {
  Blynk.run();

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid_str = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uid_str += "0";
    uid_str += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid_str.toUpperCase();

  Serial.print("UID: ");
  Serial.println(uid_str);

  if (uidEhValido(uid_str)) {
    Serial.println("✅ Acesso permitido");
    contador = contador + 1;
    Blynk.virtualWrite(V0, contador);
  } else {
    Serial.println("❌ Acesso negado");
  }
  // contador = contador + 1;
  //Blynk.virtualWrite(V0, contador);
  

  delay(1500);
}
