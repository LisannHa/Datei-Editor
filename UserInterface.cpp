//
//  UserInterface.cpp
//  CHAR-Datei-Editor
//
//  Created by Lisann Hartleb on 04.11.25.
//

#include "UserInterface.h"
#include "DataConverter.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>    // std::min
#include <cctype>       // std::isprint

using namespace std;

// --- Konstante: Bytes pro Zeile im Dump ---
static constexpr size_t BYTES_PER_LINE = 16;

// Konstruktor: Verknüpft mit FileEditor
UserInterface::UserInterface(FileEditor &fe) : m_fe(fe) {}

// Hauptprogrammschleife: Eingabe -> Auswertung -> Ausgabe
void UserInterface::run(const string &initialPath) {
// Wenn beim Start schon eine Datei angegeben wurde -> öffnen
    if (!initialPath.empty()) {
        if (m_fe.openFile(initialPath)) {
            cout << "Datei geöffnet: " << initialPath << " (" << m_fe.size() << " Bytes)\n";
        } else {
            cerr << "Fehler: Datei konnte nicht geöffnet werden.\n";
        }
    }

    showHelp();

    string line;
      while (true) {
          cout << "> ";
          if (!getline(cin, line)) break;
          if (line.empty()) continue;

          if (line == "quit" || line == "exit") break;

          handleCommand(line);
      }
  }

  // Hilfeanzeigen
  void UserInterface::showHelp() {
      cout << "Verfügbare Befehle:\n";
      cout << " open <Pfad>                         - Datei öffnen\n";
      cout << " dump [offset] [lines]               - Dump ab Offset                                             (standard: offset=0, lines=1)\n";
      cout << " edit <offset> <hex|bin|char> <wert> - Byte an Position                                                      ändern\n";
      cout << " save                                - Änderungen speichern\n";
      cout << " revert                              - Änderungen verwerfen                                                  (Datei neu laden)\n";
      cout << " info                                - Status anzeigen\n";
      cout << " help                                - Hilfe anzeigen\n";
      cout << " quit / exit                         - Programm beenden\n";
  }

  // Anzeige einer "Zeile" (eine Speicheradresse + 16 Bytes) in 3 Spalten: HEX | BIN | CHAR
  void UserInterface::displayPage(size_t offset) {
      const auto &buf = m_fe.buffer();
      const size_t n = buf.size();

      if (n == 0) {
          cout << "Keine Datei geladen.\n";
          return;
      }
      if (offset >= n) {
          cout << "Offset liegt außerhalb der Datei.\n";
          return;
      }

      const size_t end = std::min(offset + BYTES_PER_LINE, n);

      // Adresse
      cout << hex << setw(8) << setfill('0') << offset << "  " << setfill(' ');

      // 1) HEX-Spalte (16 Werte, gepadded)
      for (size_t i = offset; i < offset + BYTES_PER_LINE; ++i) {
          if (i < end) cout << DataConverter::toHex(buf[i]) << ' ';
          else cout << "   ";
      }

      cout << "| ";

      // 2) BIN-Spalte (16 Werte, gepadded)
      for (size_t i = offset; i < offset + BYTES_PER_LINE; ++i) {
          if (i < end) cout << DataConverter::toBin(buf[i]) << ' ';
          else cout << "         ";
      }

      cout << "| ";

      // 3) CHAR-Spalte (ASCII; nicht druckbar -> '.')
      for (size_t i = offset; i < end; ++i) {
          // Wenn dein DataConverter::toChar bereits "." liefert, passt es ebenfalls.
          string c = DataConverter::toChar(buf[i]);
          char out = '.';
          if (!c.empty()) out = c[0];
          cout << out;
      }

      cout << dec << "\n";
  }

  // Eingabe-Befehle verarbeiten
  void UserInterface::handleCommand(const string &line) {
      istringstream is(line);
      string cmd;
      is >> cmd;

      // ---------------- open ----------------
      if (cmd == "open") {
          string path;
          getline(is, path);
          if (!path.empty() && path.front() == ' ') path.erase(0, 1);

          if (path.empty()) {
              cout << "Verwendung: open <Pfad>\n";
              return;
          }

          // BUGFIX: Erfolgslogik korrekt herum
          if (m_fe.openFile(path))
              cout << "Datei geöffnet: " << path << "\n";
          else
              cerr << "Fehler beim Öffnen der Datei.\n";
      }

      // ---------------- dump ----------------
      else if (cmd == "dump") {
          // Optional: offset und lines
          size_t off = 0;
          size_t linesCount = 1;

          if (is >> off) {
              // optional lines
              if (!(is >> linesCount)) {
                  linesCount = 1;
              }
          } else {
              off = 0;
              linesCount = 1;
          }

          // Dump mehrere Zeilen
          for (size_t k = 0; k < linesCount; ++k) {
              size_t rowOffset = off + k * BYTES_PER_LINE;
              if (!m_fe.buffer().empty() && rowOffset >= m_fe.buffer().size()) break;
              displayPage(rowOffset);
          }
      }

      // ---------------- edit ----------------
      else if (cmd == "edit") {
          // Neues Format passend zum Text:
          // edit <offset> <hex|bin|char> <wert>
          size_t off;
          string mode, val;
          is >> off >> mode >> val;

          if (!is) {
              cout << "Verwendung: edit <offset> <hex|bin|char> <wert>\n";
              return;
          }

          uint8_t b = 0;
          bool ok = false;

          if (mode == "hex") {
              ok = DataConverter::fromHex(val, b);
          } else if (mode == "bin") {
              ok = DataConverter::fromBin(val, b);
          } else if (mode == "char") {
              if (val.empty()) ok = false;
              else {
                  b = static_cast<uint8_t>(val[0]);
                  ok = true;
              }
          } else {
              cout << "Unbekannter Modus. Nutze hex|bin|char.\n";
              return;
          }

          if (!ok) {
              cout << "Ungültiger Wert.\n";
              return;
          }

          if (!m_fe.setByte(off, b))
              cout << "Byte konnte nicht geändert werden.\n";
          else
              cout << "Byte erfolgreich geändert.\n";
      }

      // ---------------- save ----------------
      else if (cmd == "save") {
          if (m_fe.saveFile())
              cout << "Datei gespeichert.\n";
          else
              cout << "Speichern fehlgeschlagen.\n";
      }

      // ---------------- revert ----------------
      else if (cmd == "revert") {
          const string path = m_fe.filePath();
          if (path.empty()) {
              cout << "Keine Datei geöffnet.\n";
              return;
          }

          // Neu laden überschreibt Puffer -> Änderungen verworfen
          if (m_fe.openFile(path))
              cout << "Änderungen verworfen, Datei neu geladen.\n";
          else
              cout << "Neu laden fehlgeschlagen.\n";
      }

      // ---------------- info ----------------
      else if (cmd == "info") {
          cout << "Datei: " << (m_fe.filePath().empty() ? "<keine>" : m_fe.filePath()) << "\n";
          cout << "Bytes: " << m_fe.size() << "\n";
          cout << "Ungespeichert: " << (m_fe.isDirty() ? "ja" : "nein") << "\n";
          cout << "Hinweis: Anzeige im Dump erfolgt tabellarisch (HEX | BIN | CHAR).\n";
      }

      // ---------------- help ----------------
      else if (cmd == "help") {
          showHelp();
      }

      // ---------------- unbekannt ----------------
      else {
          cout << "Unbekannter Befehl. Tippe 'help' für Hilfe.\n";
      }
  }
