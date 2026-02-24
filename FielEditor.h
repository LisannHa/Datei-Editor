//
//  FielEditor.h
//  Datei-Editor
//
//  Created by Lisann Hartleb on 05.11.25.
//

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>   // size_t

using namespace std;
// Enum-Klasse zur Auswahl der Anzeigeart (HEX, BIN, CHAR)
enum class ViewMode {
    HEX,
    BIN,
    CHAR
};

class FileEditor {
public:
    FileEditor();

    bool openFile(const string &path);          // Datei öffnen & in Speicher laden
    bool saveFile();                                 // Änderungen speichern
    void closeFile();                                // Datei schließen / Puffer leeren

    size_t size() const;                             // Dateigröße in Bytes (Puffergröße)
    const vector<uint8_t>& buffer() const;      // Zugriff auf Byte-Puffer

    bool setByte(size_t offset, uint8_t value);      // Einzelnes Byte ändern
    bool writeBufferToDisk();                        // Inhalt in Datei schreiben

    string filePath() const;                    // Aktuellen Pfad zurückgeben
    bool isDirty() const;                            // Ungespeicherte Änderungen?

    ViewMode viewMode() const;                       // Aktuellen Darstellungsmodus                                                 abfragen
    void setViewMode(ViewMode m);                    // Darstellungsmodus ändern                                                    (optional)

private:
    string m_path;                              // Aktueller Dateipfad
    vector<uint8_t> m_buf;                      // Inhalt der Datei als                                                        Byte-Array
    bool m_dirty;                                    // Dirty flag: Änderungen                                                      vorhanden?
    ViewMode m_view;                                 // Modus (optional, falls noch                                                 genutzt)
};
