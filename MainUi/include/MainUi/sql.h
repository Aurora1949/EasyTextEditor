//
// Created by Jeffrey Hsu on 2023/6/27.
//

#ifndef EASYTEXTEDITOR_SQL_H
#define EASYTEXTEDITOR_SQL_H

const char* CREATE_TABLE_SETTINGS = "CREATE TABLE IF NOT EXISTS settings ( "
                                    "   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                    "   fontSize INTEGER NOT NULL DEFAULT 9,"
                                    "   fontStyle INTEGER NOT NULL DEFAULT 0,"
                                    "   fontFamily VARCHAR NOT NULL DEFAULT '宋体'"
                                    ")";

const char* CREATE_TABLE_HISTORY = "CREATE TABLE history ("
                                   "    id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
                                   "    filePath VARCHAR NOT NULL,"
                                   "    date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP"
                                   ")";

const char* SET_INIT_SETTINGS = "INSERT INTO settings (fontSize, fontStyle, fontFamily) VALUES (9, 0, '宋体')";

const char* LOAD_SETTINGS = "SELECT fontSize, fontStyle, fontFamily FROM settings LIMIT 1";

const char* SET_SETTINGS = "UPDATE settings SET (fontSize, fontStyle, fontFamily) = (?, ?, ?) WHERE id = 1";

const char* INSERT_NEW_HISTORY = "INSERT INTO history (filePath) VALUES (?)";

const char* LOAD_ALL_HISTORY = "SELECT DISTINCT filePath, MAX(date) AS max_date FROM history GROUP BY filePath ORDER BY max_date DESC LIMIT 5";

#endif //EASYTEXT;EDITOR_SQL_H
