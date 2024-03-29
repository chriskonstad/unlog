/*
Copyright (c) <2012> <Chris Konstad (chriskon149@gmail.com)>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
