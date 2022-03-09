#include "file_writer.h"

FileWriter::FileWriter(QObject *parent)
    : QObject{parent}
{
    this->is_writing_file = false;
    this->timer = new QTimer(this);
    this->timer->setTimerType(Qt::PreciseTimer);
}

void FileWriter::writeData(State &current_state) {
    file_data_queue.enqueue(current_state);
}

void FileWriter::setFilePath(QString file_path) {
    this->file_path = file_path;
}

QString FileWriter::getFilePath() {
    return this->file_path;
}

void FileWriter::startWriteFile() {
    is_writing_file = true;
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    file_path = QDir::currentPath() + "/" + date.toString("yyyyMMdd") + "-" + time.toString("hhmmss") + ".csv";
    qDebug() << file_path;
    connect(timer, SIGNAL(timeout()), this, SLOT(writeFile()));
    connect(this, SIGNAL(startFileWriterTimer()), timer, SLOT(start()));
    connect(this, SIGNAL(stopFileWriterTimer()), timer, SLOT(stop()));
    Q_EMIT startFileWriterTimer();
}

void FileWriter::stopWriteFile() {
    is_writing_file = false;
    disconnect(timer, SIGNAL(timeout()), this, SLOT(writeFile()));
    Q_EMIT stopFileWriterTimer();
    disconnect(this, SIGNAL(startFileWriterTimer()), timer, SLOT(start()));
    disconnect(this, SIGNAL(stopFileWriterTimer()), timer, SLOT(stop()));
    file_data_queue.clear();
}

void FileWriter::writeFile() {
    while(!file_data_queue.isEmpty() && is_writing_file) {
        current_state = file_data_queue.dequeue();
        string_list << QString::number(current_state.x) << QString::number(current_state.y) << QString::number(current_state.angle);
        qDebug() << "read data";
        qDebug() << QString::number(current_state.x) << "," << QString::number(current_state.y) << "," << QString::number(current_state.angle);
        string_data.addRow(string_list);
        string_list.clear();
        QtCSV::Writer::write(file_path, string_data);
    }
}
