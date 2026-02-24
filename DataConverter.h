//
//  DataConverter.h
//  CHAR-Datei-Editor
//
//  Created by Lisann Hartleb on 04.11.25.
//

#pragma once        // Header-Guard: verhindert Mehrfacheinbindung                      derselben Header-Datei
#include <string>   // std::string
#include <cstdint>  // uint8_t (Byte-Typ)

using namespace std; // ermöglicht string statt std::string

// Hilfsklasse zur Konvertierung zwischen Binärwerten, Hex-Strings und Zeichen
class DataConverter {
    public:
    // Byte->verschiedene Darstellungen (ausgabeformat)
    static string toHex(uint8_t b);
    static string toBin(uint8_t b);
    static string toChar(uint8_t b);
    
    // Textdarstellun -> Byte (Eingabe parsen/validieren)
    static bool fromHex(const string &s, uint8_t &out);
    static bool fromBin(const string &s, uint8_t &out);
};
