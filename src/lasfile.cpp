#include "lasfile.h"

LasFile::LasFile()
{

}
LasFile::LasFile(const QString &filepath)
{

    GetFileInfo(filepath);
    GetVersion();
    GetWell();
    GetCurve();
    GetASCII();
}

void LasFile::GetFileInfo(const QString &filepath)
{
    file = new QFile (filepath);
    stream = new QTextStream (file);

    QFileInfo f(*file);
    FileName = f.fileName();
    FilePath = f.absoluteFilePath();
    FileBaseName = f.baseName();
    FileSuffix = f.suffix();

    CheckCodec();
}

void LasFile::GetVersion()
{
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        Stay_to("~V");
        VERS = GetFileString("VERS.", ":");
        WRAP = GetFileString("WRAP.", ":");
        file->close();
    }
    if(VERS.contains("1.2"))VERS="1.2";
    if(VERS.contains("2.0"))VERS="2.0";
    if(VERS.contains("3.0"))VERS="3.0";
}

void LasFile::GetWell()
{
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        Stay_to("~W");
        STRT = GetFileString("STRT.M", ":");
        STOP = GetFileString("STOP.M", ":");
        STEP = GetFileString("STEP.M", ":");
        NUll = GetFileString("NULL.",  ":");
        if(VERS == "1.2")
        {
            WELL = GetFileString("WELL:", "\r\n");
            DATE = GetFileString("DATE:", "\r\n");
        }
        else
        {
            WELL = GetFileString("WELL.", ":");
            DATE = GetFileString("DATE.", ":");
        }
        file->close();
    }

    if(WELL.isEmpty())WELL = FileBaseName.remove(QRegExp("[\\s]"));
    //if(subStr.isEmpty())subStr=FileName.remove(QRegExp("[^\\d]"));
}

void LasFile::GetCurve()
{
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        Stay_to("~C");
        GetGisInfo();
        file->close();
    }
}

void LasFile::GetASCII()
{
    if ((file->exists())&&(file->open(QIODevice::ReadOnly)))
    {
        Stay_to("~A");        
        GetASCIILOG();
        file->close();
    }
}

void LasFile::GetGisInfo()
{
    QString str,EndPosstr1,EndPosstr2;
    int EndPos;

    EndPosstr1=":";
    EndPosstr2=".";

   do
    {
        if((str.contains(EndPosstr2)) && (str.indexOf(EndPosstr2) < str.indexOf(EndPosstr1))) // проверка точек и запятых
        {
            EndPos = str.indexOf(EndPosstr2);
        }
        else
        {
            EndPos = str.indexOf(EndPosstr1);
        }

        QString subStr = str.mid(0, EndPos);
        subStr.remove(QRegExp("[\\s]"));
        if(!subStr.isEmpty())GISINFO<<subStr;

        str=readline();
    }
    while((!(str.indexOf("~")==0))&&(!stream->atEnd()));
}

void LasFile::GetASCIILOG()
{
    QString str;
    QStringList List;
    int N = GISINFO.size();
    for(int i = 0; i < 2 * N; i++)
    {
        ASCIILOG.push_back(NUll);
    }
    QVector<bool>End(N, false);

    if(WRAP=="NO")
    {
        do
        {
            str=stream->readLine();
            List += str.split(" ");
            List.removeAll("");

            for(int i = 0; i < N; i++)
            {
                if(!List[i].contains(NUll))
                {
                    if(End[i] == false) // начало гиса
                    {
                        ASCIILOG[i] = List[0];
                        End[i] = true;
                    }
                    else               // конец гиса
                    {
                        ASCIILOG[i + N] = List[0];
                    }
                }
            }
            List.clear();
        }
        while(!stream->atEnd());
    }
    else
    {
        // для Wrap "yes"
    }
}
QString LasFile::GetFileString(const QString &Posstr, const QString &EndPosstr)
{
    QString str;

    do
    {
        str=readline();
    }
    while((!str.contains(Posstr))&&(!stream->atEnd()));

    int Pos =  Posstr.size() + str.indexOf(Posstr);
    int EndPos = str.indexOf(EndPosstr, Pos);
    QString subStr = str.mid(Pos, EndPos - Pos);
    return subStr.remove(QRegExp("[\\s]"));
}

void LasFile::Stay_to(const QString &string)
{
    stream->seek(0);
    QString str;
    do
    {
        str=stream->readLine();
    }
    while((!(str.indexOf(string)==0))&&(!stream->atEnd()));
}

QString LasFile::readline()
{
    QString str;
    do
    {
        str=stream->readLine();
    }
    while((str.indexOf("#")==0)&&(!stream->atEnd()));
    return str;
}


void LasFile::CheckCodec()
{
    GetCurve();

    int cod = 0;
    for(int i = 1; i < GISINFO.size(); i++)
    {
        cod += Codec(GISINFO[i]);
    }
    GISINFO.clear();

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
}

int LasFile::Codec(const QString &string)
{
    // Определение кодировки 1 - 866 -1 - 1251
/*
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


