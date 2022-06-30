#ifndef LASFILE_H
#define LASFILE_H

#include <QVector>
#include <QFileInfo>
#include <QString>
#include <QDebug>

class LasFile
{
public:
    // Информация о файле
    QString FilePath;
    QString FileName;
    QString FileBaseName;
    QString FileSuffix;

    // ~V – информация о версии и режиме отображения данных в секции
    QString VERS;
    QString WRAP;

    // ~W – информация о скважине
    QString STRT;
    QString STOP;
    QString STEP;
    QString NUll;
    QString WELL;
    QString DATB;
    QString DATE;

    //~C – информация о каротажных кривых
    QStringList GISINFO;

    //~A – содержит цифровые данные по скважине (последняя секция файла)
    QStringList ASCIILOG;

    LasFile();
    LasFile(const QString & filepath);

private:
    // Содержание файла
    void GetFileInfo(const QString & filepath);
    void GetVersion();
    void GetWell();
    void GetCurve();
    void GetASCII();

    // ~V – информация о версии и режиме отображения данных в секции
    // ~W – информация о скважине
    // ~C – информация о каротажных кривых

    void GetGisInfo();
    // ~A – содержит цифровые данные по скважине (последняя секция файла)
    void GetASCIILOG();

    // Вспомогательные для работы
    QStringList MNEM;
    QString Mneminika(const QString &string);

    QFile *file;
    QTextStream *stream;
    void Stay_to        (const QString & string);
    QString GetFileString(const QString &Posstr, const QString &EndPosstr);
    QString readline();
    
    int Codec(const QString &string);
    void CheckCodec();
};

#endif // LASFILE_H
