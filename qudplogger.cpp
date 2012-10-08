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

#include "qudplogger.h"
#include <QFile>

QUdpLogger::QUdpLogger(QList<unsigned int> ports, QString filename, bool ts, QObject *parent) :
    QObject(parent)
{
    file = new QFile(this);
    QTime time;

    timeStamp = false;
    timeStamp = ts;

    QString portsList = "Ports: ";
    foreach(unsigned int p, ports)
    {
        portsList.append(QString::number(p));
        portsList.append(", ");
    }

    //Open the log file
    openFile(filename);
    writeToFile(QString("QUdpLogger written by Chris Konstad.  Email: chriskon149@gmail.com"));
    writeToFile(time.currentTime().toString()); //Timestamp the logs
    writeToFile(portsList); //print out the connected ports

    //Open the UDP ports and connect signals/slots
    for(int i=0;i<ports.count();i++)
    {
        QUdpSocket *socket = new QUdpSocket(this);
        socket->bind(ports[i], QUdpSocket::ShareAddress);
        connect(socket, SIGNAL(readyRead()), this, SLOT(onPacketReceived()));
        sockets.append(socket);
    }
}

bool QUdpLogger::openFile(QString filename)
{
    file->close();  //close any open files
    if(!filename.isEmpty())
    {
        file->setFileName(filename);    //set the file and path
        return file->open(QIODevice::Append);    //Will either open a file or create on (if it does not exist)
    }
    else
        return false;
}

void QUdpLogger::closeFile()
{
    //qDebug() << "Closing file";
    file->close();  //close any open files
}

void QUdpLogger::writeToFile(QString text)
{
    writeToFile(text.toAscii());
}

void QUdpLogger::writeToFile(QByteArray ba)
{
    if(file->isOpen())
    {
        if(timeStamp)
        {
            QTime time;
            file->write(time.currentTime().toString("hh:mm:ss:zzz").toAscii());
            file->write(QString("\t").toAscii());
        }
        int ret = file->write(ba);    //write out the QByteArray to the file
        file->write("\n");  //carriage return
        //qDebug() << "Bytes written to file: " << ret;
        file->flush();  //actually write the bytes to the file
    }
    else if(!file->fileName().isEmpty())
    {
        file->open(QIODevice::Append);
        writeToFile(ba);
    }
    else
        qDebug() << "File is closed and filename is empty";
}

void QUdpLogger::onPacketReceived()
{

    QByteArray datagram;

    QUdpSocket *socket = qobject_cast<QUdpSocket *>(sender());

    do
    {
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());
    }
    while(socket->hasPendingDatagrams());
    writeToFile(datagram);
}
