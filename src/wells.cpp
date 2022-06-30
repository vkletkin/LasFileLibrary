#include "wells.h"


Wells::Wells(const QStringList &FilePathList)
{
    ApendtoVectors(FilePathList);
    Work_LasFiles();
    Work_InkFiles();
}

void Wells::ApendtoVectors(const QStringList &FilePathList)
{
    foreach(auto str,FilePathList)
    {
        if (QFileInfo(str).suffix() == "las")
            LasFilesVector.push_back(str);
        else
            InkFilesVector.push_back(str);
    }
}

void Wells::Work_LasFiles()
{
    if(!LasFilesVector.isEmpty()) // если список не пустой  сортируем файлы
    {
        for (int i = 0; i < LasFilesVector.size(); i++)
        {
            for (int j = i + 1; j < LasFilesVector.size(); j++)
            {
                if(CheckDuplicate(LasFilesVector[i], LasFilesVector[j]))
                {
                    LasFilesVector.removeAt(j);
                    j--;
                }
            }
        }

        if(!LasFilesVector.isEmpty())
        {
            for (int i = 0; i < LasFilesVector.size(); i++)
            {
                if(LasFilesVector[i].WELL.isEmpty())
                {
                    LasFilesVector.removeAt(i);
                    i--;
                }
            }
        }
    }
}


bool Wells::CheckDuplicate(const LasFile &left, const LasFile &right)
{
    if(left.WELL==right.WELL)
    {
        if((left.STEP==right.STEP)&&(left.STOP==right.STOP)&&(left.STRT==right.STRT)&&(left.GISINFO==right.GISINFO))
        {
            return true; // это говорит что они одинаковые
        }
        else
        {
            return false; // это означает что они описывают одну и туже скважину но с разными данными
        }
    }
    else
    {
        return false; // это означает что они разные начиная с названия
    }
}


void Wells::Work_InkFiles()
{
    if(!InkFilesVector.isEmpty())
    {
        for (int i = 0; i < InkFilesVector.size(); i++)
        {
            if(InkFilesVector[i].WELL.isEmpty())
            {
                InkFilesVector.removeAt(i);
                i--;
            }
        }
    }
}

void Wells::CopyLasToFolder(const QString &Folder, const LasFile &lasfile, int i)
{
    QString newFilePath = lasfile.WELL;

    foreach (auto gis, lasfile.GISINFO)
    {
        if((gis!="DEPT")&&(gis!="DEPTH"))
            newFilePath += "_" + gis;
    }

    if(i == 0)
    {
        newFilePath += "." + lasfile.FileSuffix;
    }
    else
    {
        QString ARG = QString::number(i);
        newFilePath += "_" + ARG + "." + lasfile.FileSuffix;
    }

    newFilePath.remove(QRegExp("[:*?<>|+]"));// удаление из нового имени  данных символов / : * ? " < > | +
    newFilePath = Folder + "/" + newFilePath;

    if(QFile::exists(newFilePath))
    {
        CopyLasToFolder(Folder,lasfile, i+1);
    }
    else
    {
        QFile::copy(lasfile.FilePath, newFilePath);
    }
}

void Wells::CopyInkToFolder(const QString &Folder, InkFile &inkfile, int i)
{

    QString newFilePath = inkfile.WELL;

    if(i == 0)
    {
        newFilePath += ".txt";
    }
    else
    {
        QString ARG = QString::number(i);
        newFilePath += "_" + ARG + ".txt";
    }

    newFilePath.remove(QRegExp("[:*?<>|+]"));// удаление из нового имени  данных символов / : * ? " < > | +
    newFilePath = Folder + "/" + newFilePath;

    if(QFile::exists(newFilePath))
    {
        CopyInkToFolder(Folder, inkfile, i+1);
    }
    else
    {
        inkfile.CreateInk(newFilePath);
    }
}


void Wells::CopyLasTo(const QString &Folder)
{
    foreach(auto Las,LasFilesVector)
    {
        CopyLasToFolder(Folder,Las);
    }
}

void Wells::CopyInkTo(const QString &Folder)
{
    foreach(auto Ink,InkFilesVector)
    {
        CopyInkToFolder(Folder, Ink);
    }
}

void Wells::CreateTable(const QString &Folder)
{
    QXlsx::Document xlsx;
    QXlsx::Format format;
    QXlsx::CellRange range;

    format.setFontSize(10);
    format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format.setBorderStyle(QXlsx::Format::BorderThin);

    int A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8;

    xlsx.write("A1", "Скв");
    xlsx.mergeCells("A1:A2", format);
    xlsx.write("B1", "Дата");
    xlsx.mergeCells("B1:B2", format);
    xlsx.write("C1", "X Устья");
    xlsx.mergeCells("C1:C2", format);
    xlsx.write("D1", "Y Устья");
    xlsx.mergeCells("D1:D2", format);
    xlsx.write("E1", "Альтитуда");
    xlsx.mergeCells("E1:E2", format);

    //format.setHorizontalAlignment(QXlsx::Format::AlignHDistributed);
    xlsx.write("F1", "Наличие инклинометра");
    // format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    xlsx.mergeCells("F1:G2", format);
    xlsx.write("H1", "Методы ГИС");


    QStringList List;

     foreach(auto Las,LasFilesVector)
     {
         for (int j = 1; j < Las.GISINFO.size(); j++)
         {
             if(!List.contains(Las.GISINFO[j])) // тут может быть сложный момент
             {
                 List<<Las.GISINFO[j];
             }
         }
     }

     for (int i = 0; i < List.size(); i++)
     {
         int Stroka = 2,  Stolbec = H + 2 * i;

         xlsx.write( Stroka, Stolbec, List[i]);
         range.setFirstRow(Stroka);
         range.setFirstColumn(Stolbec);
         range.setLastRow(Stroka);
         range.setLastColumn(Stolbec + 1);
         xlsx.mergeCells(range, format);

         if(i == List.size() - 1)
         {
             Stroka = 1, Stolbec = H + 2 * i;

             range.setFirstRow(Stroka);
             range.setFirstColumn(H);
             range.setLastRow(Stroka);
             range.setLastColumn(Stolbec + 1);
             xlsx.mergeCells(range, format);
         }
     }

     for (int i = 0; i < LasFilesVector.size(); i++)
     {
         int Stroka = i + 3,  Stolbec;
         xlsx.write(Stroka, C, "", format);
         xlsx.write(Stroka, D, "", format);
         xlsx.write(Stroka, E, "", format);
         xlsx.write(Stroka, F, "", format);
         xlsx.write(Stroka, G, "", format);

         for (int j = 0; j < List.size(); j++)
         {
             Stolbec = H + 2 * j;
             xlsx.write(Stroka, Stolbec, "" ,format);
             xlsx.write(Stroka, Stolbec + 1,"",format);
         }

         xlsx.write(Stroka, A, LasFilesVector[i].WELL, format); // заполнение скв
         xlsx.write(Stroka, B, LasFilesVector[i].DATE, format); // заполнение год

         for (int j = 0; j < InkFilesVector.size(); j++)
         {
             if(InkFilesVector[i].WELL == LasFilesVector[i].WELL)
             {
                 xlsx.write(Stroka, C, InkFilesVector[i].X, format); // заполнение X устья
                 xlsx.write(Stroka, D, InkFilesVector[i].Y, format); // заполнение Y устья
                 xlsx.write(Stroka, E, InkFilesVector[i].ALTI, format); // заполнение Альтитуда
                 xlsx.write(Stroka, F, InkFilesVector[i].INKL[0], format); // заполнение Инклометра
                 xlsx.write(Stroka, G, InkFilesVector[i].INKL[1], format); // заполнение Инклометра
                 InkFilesVector.removeAt(i);
                 break;
             }
         }

         for (int j = 0; j < List.size(); j++)                  // заполнение методы гис
         {
             Stolbec = H + 2 * j;

             for (int k = 1; k < LasFilesVector[i].GISINFO.size(); k++)
             {
                 if(LasFilesVector[i].GISINFO[k] == List[j])
                 {
                     xlsx.write(Stroka, Stolbec,  LasFilesVector[i].ASCIILOG[k],                                    format);
                     xlsx.write(Stroka, Stolbec+1,LasFilesVector[i].ASCIILOG[k + LasFilesVector[i].GISINFO.size()], format);
                 }
             }
         }
     }

     /*
     for (int i = 0; i < InkFilesVector.size(); i++)
     {
         int Stroka = i + 3, Stolbec;

         for (int j = 0; j < ; j++)
         {
             if(InkFilesVector[i].WELL == LasFilesVector[i].WELL)
             {
                 xlsx.write(Stroka, C, InkFilesVector[i].X, format); // заполнение X устья
                 xlsx.write(Stroka, D, InkFilesVector[i].Y, format); // заполнение Y устья
                 xlsx.write(Stroka, E, InkFilesVector[i].ALTI, format); // заполнение Альтитуда
                 xlsx.write(Stroka, F, InkFilesVector[i].INKL[0], format); // заполнение Инклометра
                 xlsx.write(Stroka, G, InkFilesVector[i].INKL[1], format); // заполнение Инклометра
                 InkFilesVector.removeAt(i);
                 break;
             }
             else
             {

             }
         }
     }
*/

    xlsx.saveAs(Folder+"/Таблица.xlsx");
}

