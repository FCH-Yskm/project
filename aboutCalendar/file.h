
#ifndef FILE_H
#define FILE_H

// 文件
#include <QFile>
#include <QTextStream>
// 调试
#include <QDebug>

class file
{
public:
    QString content;
    QStringList lines;
    QStringList headers;
    QList<QStringList> units;

    int content_rows_num=0;
    int content_columns_num=5;

    QString SpiltSymbol = ",";

    // .dat .csv .txt
    void read_matrix(const QString & filename,QString event);
    void write_matrix(const QString & filename,QString event);
private:


};

#endif // FILE_H
