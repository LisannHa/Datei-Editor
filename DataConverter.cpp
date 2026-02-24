//
//  DataConverter.cpp
//  CHAR-Datei-Editor
//
//  Created by Lisann Hartleb on 04.11.25.
//

#include "DataConverter.h"     // Einbindung der eigenen                                           Header-Deklarationen
#include <sstream>              // stringstream für                                                 Formatierung/Parsing
#include <bitset>
#include <iomanip>              // setw, setfill, hex, uppercase

using namespace std;

// Byte -> Hexadezimaldarstellung (z.B. 4F)
string DataConverter::toHex(uint8_t b) {
    stringstream ss;
    ss << hex                   // Ausgabe in Hexadezimalbasis
        << uppercase            // String-Stream als                                        "Text-Ausgabepuffer"
        << setw(2)              // mindestens 2 Zeichen breit
        << setfill('0')         // führende Nullen, falls nötig
        << static_cast<int>(b); // uint8_t wird als char                                    interpretiert
    return ss.str();            // formatierten String zurückgegebn
}

//Byte -> Binärdarstellung (z.B. 01001111)
string DataConverter::toBin(uint8_t b) {
    string s;                               //Ergebnisstring
    for (int i = 7; i >= 0; --i)            //Bits von 7 bis 0
        s += ((b >> i) & 1) ? '1' : '0';
    return s;                               //8 Zeichen String                                              zurückgeben
}

// Byte -> Zeichen (druckbar oder Punkt)
string DataConverter::toChar(uint8_t b) {
    //ASCII: 32..126 gelten als druckbar
    //byte -> char -> string mit Länge 1
    if (b >= 32 && b <= 126) return string(1, (char)b);
        return ".";     //nicht-druckbare Werte werden als Punkt                    dargestellt
   }

// Hex-String -> Byte
bool DataConverter::fromHex(const string &s, uint8_t &out) {
    //Hex-Eingabe muss genau 2 Zeichen lang sein
    if (s.size() != 2) return false;
    unsigned int val;       //temporär größerer Typ für Parsing
    stringstream ss;        //Parsing-Stream
    ss << hex << s;         //stream auf Hex-Modus setzen und string                        hineinschreiben
    if (!(ss >> val))       //wenn Parsing fehlschlägt -> ungültig
        return false;
    out = static_cast<uint8_t>(val);       //nur die unteren 8 Bits                                         übernehmen
    return true;            //Parsing ok
}

// Binärstring -> Byte
bool DataConverter::fromBin(const string &s, uint8_t &out) {
    if (s.size() != 8)
        return false;       //Binärstring muss 8 Bits haben
    uint8_t val = 0;        //Ergebnisbyte
    for (char c : s) {      //jedes Zeichen durchlaufen
        if (c != '0' && c != '1') return false; //nur 0/1 erlaubt
        val = (val << 1) | (c - '0'); //links schieben und neues Bit                                anhängen
    }
    out = val;
    return true;
}
