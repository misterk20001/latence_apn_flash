#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);
int apn = 10;             //pin apn
int flash = 11;           // pin flash/led
int buttonState = 0;      //etat du bouton
const int buttonPin = 2;  // pin bouton poussoir
int latence = 50;
int cliches = 5;
int inc_latence = 5;
String lecture_moniteur = "";
int onoff = 1;

void setup() {
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40, 0);
  display.display();

  Serial.begin(9600);

  pinMode(apn, OUTPUT);
  pinMode(flash, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  debut();
}
void loop() {
   

  buttonState = digitalRead(buttonPin);  //lecture bouton


  if (buttonState == HIGH) {  //si le bouton poussoir est préssé  : lancement de la sequence
    for (int i = 1; i <= cliches; i++) {
      // Display Text
      display.clearDisplay();
      display.setTextSize(3);
      display.setTextColor(WHITE);
      display.setCursor(40, 0);
      display.println(latence);
      display.display();
      if (onoff == 1) {
        digitalWrite(apn, HIGH);  // déclenchement appareil photo pour relevage miroir
        delay(30);                // attende pour calcul de la latence
        digitalWrite(apn, LOW);   // remise a zero declencheur
        Serial.print("miroir relevé");
      }
      delay(100);
      digitalWrite(apn, HIGH);    // déclenchement appareil photo
      delay(latence);             // attende pour calcul de la latence
      digitalWrite(flash, HIGH);  // déclenchement flash
      digitalWrite(apn, LOW);     // exctinction appareil photo
      delay(1);                   // durée de l allumage de la led
      digitalWrite(flash, LOW);   //exctinction de la led

      latence = latence + inc_latence;
      delay(1500);
    }
  }
}

void debut() {
  // Display Text
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(40, 0);
      display.println("entrez");
      display.println("    les parametres");
      display.display();
  Serial.print("relevage du miroir? (o/n)  : ");  //interrogation moniteur serie miroir relevé
  while (Serial.available() == 0)
    ;
  {
    delay(10);
    lecture_moniteur = Serial.readString();
    lecture_moniteur.trim();  // enleve \r \n espace blanc a la fin de la ligne
    if (lecture_moniteur == "o") {
      onoff = 1;                         //relevé
      Serial.println(lecture_moniteur);  //affichage moniteur reponse oui non
    } else {
      onoff = 0;                         //baissé
      Serial.println(lecture_moniteur);  //affichage moniteur reponse oui non
    }

    Serial.print("début de la premiere valeur de latence? : ");  //interrogation moniteur valeur de latence
    while (Serial.available() == 0)
      ;
    {
      delay(10);
      lecture_moniteur = Serial.readString();
      lecture_moniteur.trim();             // enleve \r \n espace blanc a la fin de la ligne
      latence = lecture_moniteur.toInt();  // conversion caractere/nombre
      Serial.println(latence);             //affichage moniteur serie nombre de clichés
    }
  }

  Serial.print("nombre de clichés? : ");  //interrogation moniteur serie nombre de clichés
  while (Serial.available() == 0)
    ;
  {
    delay(10);
    lecture_moniteur = Serial.readString();
    lecture_moniteur.trim();             // enleve \r \n espace blanc a la fin de la ligne
    cliches = lecture_moniteur.toInt();  // conversion caractere/nombre
    Serial.println(cliches);             //affichage moniteur serie nombre de clichés
  }
  Serial.print("incrémentation de latence ? (en millisecondes) : ");  //interrogation moniteur serie incrémentation de latence
  while (Serial.available() == 0)
    ;
  {
    delay(10);
    lecture_moniteur = Serial.readString();
    lecture_moniteur.trim();                 // // enleve \r \n espace blanc a la fin de la ligne
    inc_latence = lecture_moniteur.toInt();  // conversion caractere/nombre
    Serial.println(inc_latence);             //affichage moniteur serie increment de latence
  }
  Serial.println("prêt pour le déclenchement");  //affichage moniteur serie increment de latence
  display.clearDisplay();
  display.print("READY");
  display.display();
}
