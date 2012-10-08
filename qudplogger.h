#ifndef QUDPLOGGER_H
#define QUDPLOGGER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QtNetwork>
#include <QByteArray>
#include <QList>
#include <QTime>
#include <QtDebug>

class QUdpLogger : public QObject
{
    Q_OBJECT
public:
    explicit QUdpLogger(QList<unsigned int> ports, QString filename, bool ts, QObject *parent = 0);

signals:
    
public slots:
    bool openFile(QString filename);
    void closeFile();
    void writeToFile(QString text);
    void writeToFile(QByteArray ba);
    void onPacketReceived();
    
private:
    QFile *file;
    QList<QUdpSocket*> sockets;
    bool timeStamp;
};

#endif // QUDPLOGGER_H
