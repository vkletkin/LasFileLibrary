#ifndef INKFILE_H
#define INKFILE_H

#include <QFileInfo>
#include <QString>
#include <ActiveQt/QAxWidget>
#include <ActiveQt/QAxObject>
#include <QDebug>

class InkFile
{
public:

    QString FilePath;
    QString FileName;
    QString FileBaseName;
    QString FileSuffix;

    QString WELL;
    QString DIAM;
    QString SHOE;
    QString ANGL;
    QString ALTI;
    QString ZABO;

    QString X;
    QString Y;

    QStringList INKL;
    QStringList ANSWER;

    InkFile();
    InkFile(const QString &filepath);
    void CreateInk     (const QString &newFilePath);

private:
    //содержание файла
    void GetFileInfo(const QString &newFilePath);
    void ReadFile();

    void readTXT();
    void readWord();


    //Вспомогательные методы
    QFile *file;
    QTextStream *stream;

    QString GetFileString(const QString &Posstr, const QString &EndPosstr, int i = 0);
    QString GetWordString(const QString &Posstr, const QString &EndPosstr, int i = 0);
    int Codec(const QString &string);
    void CheckCodec ();

};

#endif // INKFILE_H
