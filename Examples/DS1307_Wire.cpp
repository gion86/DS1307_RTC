//includo la libreria per la
//comunicazione su bus I2C
#include <Wire.h>
 
void setup()
{
//inizializzo la seriale
Serial.begin(9600);
 
//inizializzo la libreria
Wire.begin();
 
//attivo la comunicazione con il DS1307
//l'indirizzo dell'RTC è 0x68
Wire.beginTransmission(0x68);
//il primo byte stabilisce il registro
//iniziale da scivere
Wire.write((byte)0x00);
//specifico il tempo e la data
Wire.write((byte)0x00); //1° byte SECONDI da 0x00 a 0x59
Wire.write((byte)0x10); //2° byte MINUTI da 0x00 a 0x59
Wire.write((byte)0x80 | 0x10); //3° byte ORE da 0x00 a 0x24
Wire.write((byte)0x01); //4° byte GIORNO della settimana da 0x01 a 0x07
Wire.write((byte)0x21); //5° byte GIORNO del mese da 0x00 a 0x31
Wire.write((byte)0x03); //6° byte MESE da 0x00 a 0x12
Wire.write((byte)0x12); //7° byte ANNO 0x00 a 0x99
Wire.endTransmission();
}
 
void loop()
{
//inizzializza la trasmissione partendo
//dall'indirizzo 0x00
Wire.beginTransmission(0x68);
Wire.write((byte)0x00);
Wire.endTransmission();
 
//richiedo 7 byte dal dispositivo con
//indirizzo 0x68
Wire.requestFrom(0x68, 7);
//regupero i 7 byte relativi ai
//corrispondenti registri
byte secondi = Wire.read();
byte minuti = Wire.read();
byte ora = Wire.read();
byte giorno_sett = Wire.read();
byte giorno_mese = Wire.read();
byte mese = Wire.read();
byte anno = Wire.read();
 
Serial.print("Orario corrente: ");
Serial.print(ora, HEX);
Serial.print(":");
Serial.print(minuti, HEX);
Serial.print(":");
Serial.println(secondi, HEX);
 
Serial.print("Giorno della settimana: ");
Serial.println(giorno_sett, HEX);
 
Serial.print("Data corrente: ");
Serial.print(giorno_mese, HEX);
Serial.print("/");
Serial.print(mese, HEX);
Serial.print("/");
Serial.println(anno, HEX);
Serial.println();
 
delay(1000);
}
