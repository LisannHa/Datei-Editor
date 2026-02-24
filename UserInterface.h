//
//  UserInterface.h
//  CHAR-Datei-Editor
//
//  Created by Lisann Hartleb on 04.11.25.
//

#pragma once

#include "FileEditor.h"
#include <string>

using namespace std;

// Klasse für die Benutzerinteraktion (Eingabe und Ausgabe im Terminal)
class UserInterface {
public:
    explicit UserInterface(FileEditor &fe);
    void run(const string &initialPath = ""); // Hauptschleife starten
    
private:
    FileEditor &m_fe;                       // Referenz auf FileEditor
    void showHelp();                        // Hilfe anzeigen
    void displayPage(size_t offset = 0);     // Eine Zeile Dump anzeigen (16 Bytes                                          ab Offset)
    void handleCommand(const string &line);  // Eingaben verarbeiten
};
