#include "inkfile.h"

InkFile::InkFile()
{

}

InkFile::InkFile(const QString &filepath)
{
    GetFileInfo(filepath);
    ReadFile();
}


void InkFile::GetFileInfo(const QString &filepath)
{
    file = new QFile (filepath);
    stream = new QTextStream (file);

    QFileInfo f(*file);
    FileName=f.fileName();
    FilePath=f.absoluteFilePath();
    FileBaseName=f.baseName();
    FileSuffix=f.suffix();

    CheckCodec();
}

void InkFile::ReadFile()
{
    if(FileSuffix.contains("txt", Qt::CaseInsensitive))
      {
        readTXT();
      }
      else
      {
        readWord();
      }
}

void InkFile::readTXT()
{
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        WELL = GetFileString("Скважина"     ,"Площадь");
        if(WELL.indexOf("N")==0)WELL.remove("N");
        DIAM = GetFileString("скважины:"    ,"м.");
        SHOE = GetFileString("башмака:"     ,"м.");
        ANGL = GetFileString("склонения:"   ,"град");
        ALTI = GetFileString("Альтитуда:"   ,"м.");
        ZABO = GetFileString("Забой:"       ,"м");
        QString str = GetFileString("В интервале печати:"       ,"!!");
        INKL << str.mid(0, str.indexOf("м."));
        INKL << str.mid(str.indexOf("-") + 1 , str.indexOf("м.", str.indexOf("-")) - (str.indexOf("-") + 1));
        file->close();
    }

}

QString InkFile::GetFileString(const QString &Posstr, const QString &EndPosstr, int i)
{
    stream->seek(i);

    QString str;

    do
    {
        str=stream->readLine();
    }
    while((!str.contains(Posstr, Qt::CaseInsensitive))&&(!stream->atEnd()));

    int Pos =  Posstr.size() + str.indexOf(Posstr);
    int EndPos = str.indexOf(EndPosstr, Pos);
    QString subStr = str.mid(Pos, EndPos - Pos);
    return subStr.remove(QRegExp("[\\s]"));
}

QString InkFile::GetWordString(const QString &Posstr, const QString &EndPosstr, int i)
{
    QString str;
    do
    {
        str = ANSWER.at(i);
        i++;
        if(i== 50)break;
    }
    while((!str.contains(Posstr, Qt::CaseInsensitive))&&( i < ANSWER.size()));

    int Pos =  Posstr.size() + str.indexOf(Posstr);
    int EndPos = str.indexOf(EndPosstr, Pos);
    QString subStr = str.mid(Pos, EndPos - Pos);
    return subStr.remove(QRegExp("[\\s]"));
}


void InkFile::readWord()
{
    /*
    QAxWidget *word = new QAxWidget("Word.Application", 0, Qt::MSWindowsOwnDC);
    word->setProperty("DisplayAlerts", false);
    word->setProperty("Visible", false);

    QAxObject *documents = new QAxObject;
    documents = word->querySubObject("Documents");

    documents->dynamicCall("Open(const QString&)", FilePath);

    QAxObject *document = word->querySubObject("ActiveDocument");

    QAxObject *pRange = document->querySubObject("Range()");
    pRange->dynamicCall("WholeStory()");

    QStringList ANSWER = pRange->property("Text").toString().split('\r');

    document->dynamicCall("Close (boolean)", false);
    word->dynamicCall("Quit(void)");

    WELL = GetWordString("Скважина"     ,"Площадь");
    if(WELL.indexOf("N")==0)WELL.remove("N");
    DIAM = GetWordString("скважины:"    ,"м.");
    SHOE = GetWordString("башмака:"     ,"м.");
    ANGL = GetWordString("склонения:"   ,"град");
    ALTI = GetWordString("Альтитуда:"   ,"м.");
    ZABO = GetWordString("Забой:"       ,"м");
    QString str = GetWordString("В интервале печати:"       ,"!!");
    INKL << str.mid(0, str.indexOf("м."));
    INKL << str.mid(str.indexOf("-") + 1 , str.indexOf("м.", str.indexOf("-")) - (str.indexOf("-") + 1));
    */
}

int InkFile::Codec(const QString &string)
{
    /*
    // Определение кодировки 1 - 866 -1 - 1251

    const char *p;
    int ret = -1, i, d = 0, w = 0;
    for(; *p; p++)
    {
        if (128 <= *p && *p <160) d++;
        else if (192 <= *p && *p < 224) w++;
        else if (240 <= *p) w++;
    }
    if (d > w) ret = 1;
    return ret;
    */
    return 1;
}

void InkFile::CheckCodec()
{
    int cod = 0;

    if(cod >= 0)
    {
        if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
        {
            stream->setCodec("IBM866");
            file->close();
        }
    }
    else
    {
        if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
        {
            stream->setCodec("Windows-1251");
            file->close();
        }
    }
    stream->setCodec("CP1251");
}

void InkFile::CreateInk(const QString &newFilePath)
{
    if(FileSuffix.contains("txt", Qt::CaseInsensitive))
      {
        QFile filenew(newFilePath);
        QTextStream streamnew(&filenew);

        if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
        {
            GetFileString("Удлине","Смещен");
            GetFileString("--------", "-------- ", stream->pos());
            QString str;
            if (filenew.open(QIODevice::WriteOnly))
            {
                do
                {
                    if(!str.isEmpty())
                    {
                        streamnew<<str<<endl;
                    }
                    str = stream->readLine();
                }
                while((!(str.indexOf("--")==0))&&(!stream->atEnd()));

                filenew.close();
            }
            file->close();
        }
      }
      else
      {
         // сдесь создание файла из вордовского
      }
}

