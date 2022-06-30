#ifndef WELLS_H
#define WELLS_H

#include "lasfile.h"
#include "inkfile.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxworksheet.h"
#include "xlsxdatavalidation.h"

#include <QStringList>
#include <QVector>

class Wells
{
public:

    Wells               (const QStringList &FilePathList);
    void ApendtoVectors (const QStringList &FilePathList);
    void CopyLasTo      (const QString &Folder);
    void CopyInkTo      (const QString &Folder);
    void CreateTable    (const QString &Folder);

private:
    QVector <LasFile> LasFilesVector;
    QVector <InkFile> InkFilesVector;

    void Work_LasFiles();
    void Work_InkFiles();

    void CopyLasToFolder(const QString &Folder, const LasFile &lasfile, int i = 0);
    void CopyInkToFolder(const QString &Folder,       InkFile &Inkfile, int i = 0);

    // вспомогательные функции
    bool CheckDuplicate(const LasFile &left, const LasFile &right);
};

#endif // WELLS_H
