#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QtXml>

#include "stdio.h"
QString getID(QString);
QString getEXT(QString);

int main(int argc, char *argv[])
{
    printf("Hello world\n");
    if(2== argc){
        QString path = QString::fromStdString(argv[1]);
        QFileInfo finfo(path);
        if(finfo.exists() && finfo.isFile()){
            QFile f(path);
            if(!f.open(QIODevice::ReadOnly))
                return -1;
            QTextStream in(&f);
            QString xmlfilename = finfo.baseName() + ".xml";
            QFile xmlf(xmlfilename);
            if(!xmlf.open(QIODevice::ReadWrite))
                return -2;
            QXmlStreamWriter out(&xmlf);
            out.setAutoFormatting(true);
            out.setCodec("UTF-8");
            //out.writeDefaultNamespace("openbr");
            out.writeStartDocument();
            out.writeStartElement("biometric-signature-set");
            int i = 0;
            while(!in.atEnd()){
                QString line = in.readLine();
                i++;
                // process line, for me
                QString id = getID(line);
                QString ext  = getEXT(line);
                out.writeStartElement("biometric-signature");

                out.writeAttribute("name", id);
                out.writeEmptyElement("presentation");
                out.writeAttribute("name", QString::number(i));
                out.writeAttribute("modality", "face");
                out.writeAttribute("file-name", line);
                out.writeAttribute("file-format", ext);
                out.writeEndElement();
            }
            out.writeEndDocument();
        }
    }
    return 0;
}

QString getID(QString line){
    int idx = line.indexOf('d');
    QString ID = line.mid(0, idx);
    return ID;
}

QString getEXT(QString name){
    int idx = name.lastIndexOf('.');
    QString ext = name.mid(idx + 1, name.length() - idx);
    return ext;
}
