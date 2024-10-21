
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <filesystem>
#include <QDir>
void check_log_dir();
void InitLogs();
void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);