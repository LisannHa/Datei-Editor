//
//  FielEditor.cpp
//  Datei-Editor
//
//  Created by Lisann Hartleb on 05.11.25.
//
#include "FileEditor.h"
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

// Konstruktor: setzt Anfangswerte
FileEditor::FileEditor()
    : m_path(),
      m_buf(),
      m_dirty(false),
      m_view(ViewMode::HEX)
{}

// Datei öffnen und in Speicher laden
bool FileEditor::openFile(const std::string &path) {
    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        cerr << "Fehler: Datei konnte nicht geöffnet werden.\n";
        return false;
    }

    // Dateiinhalt in den Vektor einlesen (Byte für Byte)
    m_buf.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

    m_path = path;
    m_dirty = false;

    return true;
}

// Datei speichern (Puffer -> Festplatte)
bool FileEditor::saveFile() {
    if (m_path.empty()) {
        cerr << "Keine Datei geöffnet.\n";
        return false;
    }
    return writeBufferToDisk();
}

// Schreiboperation (Puffer wird zurück in Datei geschrieben)
bool FileEditor::writeBufferToDisk() {
    ofstream file(m_path, ios::binary | ios::trunc);
    if (!file.is_open()) {
        cerr << "Fehler: Datei konnte nicht zum Schreiben geöffnet werden.\n";
        return false;
    }

    if (!m_buf.empty()) {
        file.write(reinterpret_cast<const char*>(m_buf.data()), static_cast<std::streamsize>(m_buf.size()));
    }

    if (!file.good()) {
        cerr << "Fehler beim Schreiben der Datei.\n";
        return false;
    }

    m_dirty = false;
    return true;
}

// Datei schließen
void FileEditor::closeFile() {
    m_buf.clear();
    m_path.clear();
    m_dirty = false;
}

// Dateigröße in Bytes (Puffergröße)
size_t FileEditor::size() const {
    return m_buf.size();
}

// Zugriff auf Puffer
const vector<uint8_t>& FileEditor::buffer() const {
    return m_buf;
}

// Byte an bestimmtem Offset ändern
bool FileEditor::setByte(size_t offset, uint8_t value) {
    if (offset >= m_buf.size()) {
        cerr << "Fehler: Offset außerhalb der Datei.\n";
        return false;
    }

    m_buf[offset] = value;
    m_dirty = true;
    return true;
}

// Gibt den aktuellen Dateipfad zurück
std::string FileEditor::filePath() const {
    return m_path;
}

// Aktueller Änderungsstatus zurückgeben
bool FileEditor::isDirty() const {
    return m_dirty;
}

// Setzt den Anzeigemodus (optional nutzbar, z.B. falls UI ihn noch benötigt)
void FileEditor::setViewMode(ViewMode m) {
    m_view = m;
}

// Gibt den aktuellen Anzeigemodus zurück
ViewMode FileEditor::viewMode() const {
    return m_view;
}
