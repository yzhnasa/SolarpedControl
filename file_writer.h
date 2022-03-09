#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QQueue>
#include <QSemaphore>
#include <QString>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QMutex>
//#include <QTextStream>
#include "qtcsv/stringdata.h"
#include "qtcsv/writer.h"
#include "utilities.h"


class FileWriter : public QObject
{
    Q_OBJECT
public:
    explicit FileWriter(QObject *parent = nullptr);
    void writeData(State &current_state);
    void setFilePath(QString file_path);
    QString getFilePath();

private:
    AsyncQueue<State> file_data_queue;
    QTimer *timer;
    bool is_writing_file;
    State current_state;
    QString file_path;
    QStringList string_list;
    QtCSV::StringData string_data;

public Q_SLOTS:
    void startWriteFile();
    void stopWriteFile();
    void writeFile();

Q_SIGNALS:
    void startFileWriterTimer();
    void stopFileWriterTimer();
};

#endif // FILEWRITER_H
