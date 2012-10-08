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
