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

#include <QCoreApplication>
#include "qudplogger.h"
#include <QtDebug>
#include <QStringList>
#include <QString>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion("1.0");

    QStringList arguments = QCoreApplication::arguments();
    
    bool timeStamp = false;
    unsigned int port = 0;
    QString filePath = QDir::homePath();
    bool createObject = false;

    foreach(QString string, arguments)  //check for time stamping or help
    {
        if(string == "-h" || string == "--help")    //if help is passed
        {
            qDebug() << "";
            qDebug() << "This application, Udp Network LOGger, can listen on a specified UDP port and print any packets it receives to a specified file.";
            qDebug() << "Written by Chris Konstad (chriskon149@gmail.com) Oct. 7, 2012";
            qDebug() << "Version: " << a.applicationVersion();
            qDebug() << "";
            qDebug() << "Usage: unlog [-t] port file";
            qDebug() << "";
            qDebug() << "Arguments: ";
            qDebug() << "-t: Timestamp each packet";
            qDebug() << "port: Specify the UDP port to listen on";
            qDebug() << "file: Specify the textfile to print out to (absolute file path)";
            qDebug() << "--version: Display the current version";
            qDebug() << "-h or --help: Display the help information";

            return 0;   //quit the application
        }
        else if(string == "-t")
        {
            timeStamp = true;
        }
        else if(string == "--version")
        {
            qDebug() << "unlog version: " << a.applicationVersion();
            return 0;   //quit
        }
    }

    if(arguments.count() == 4)  //everything's here!
    {
        createObject = true;
        port = arguments.at(2).toInt(); //get the port
        QDir dir;
        dir.setPath(arguments.at(3));
        if(dir.isAbsolute())
            filePath = arguments.at(3);
        else
            filePath.append("/unlog.txt");
    }
    else if(arguments.count() == 3) //no timestamping
    {
        createObject = true;
        port = arguments.at(1).toInt(); //get the port
        QDir dir;
        dir.setPath(arguments.at(2));
        if(dir.isAbsolute())
            filePath = arguments.at(2);
        else
            filePath.append("/unlog.txt");
    }
    else
    {
        qDebug() << "Not enough arguments.  Try unlog --help";
        return 0;   //quit
    }
        QList<unsigned int> ports;
        ports.append(port);
        QUdpLogger logger(ports, filePath, timeStamp);

    return a.exec();
}
