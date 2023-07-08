#include "file.h"
void file::read_matrix(const QString &filename, QString event)
{
    // -----初始化-----
    content.clear();
    lines.clear();
    headers.clear();
    units.clear();
    content_rows_num=-1;

    // -----读取-----
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qInfo()<<filename<<"open failed";
    if(event == "二进制文件")// if .dat
    {
        QByteArray ba_data = file.readAll();
        QTextStream stream(&ba_data);
    }
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString tempLine = stream.readLine();
        // 行
        lines.append(tempLine);
        if(content_rows_num==-1)
        {
            // 表头
            headers = tempLine.split(SpiltSymbol);
        }
        else
        {
            // 数据
            units.append(tempLine.split(SpiltSymbol));
        }
        content_rows_num+=1;
    }
    // 全部内容
    content = lines.join("\n");
    // 列数
    content_columns_num = headers.count();

    // -----输出-----
    qInfo()<<"lines:\n"<<lines;
    qInfo()<<"headers:\n"<<headers;
    qInfo()<<"units:\n"<<units;
    qInfo()<<"content:\n"<<content;
    qInfo()<<"content_rows_num:"<<content_rows_num;
    qInfo()<<"content_columns_num:"<<content_columns_num;
}

void file::write_matrix(const QString &filename, QString event)
{
    // -----初始化-----
    QStringList newLines;
    for (int i = 0; i < content_rows_num+1; i++) {//+1为表头
        newLines.append(QString(""));
    }
    // -----构建-----
    // 表头
    newLines[0]= headers.join(",");
    // 数据
    for(int i=0;i<content_rows_num;i++)
    {
        for(int j=0;j<content_columns_num;j++){
            newLines[i+1].push_back(units[i][j]);
            if(j!=content_columns_num-1){
                newLines[i+1].push_back(SpiltSymbol);
            }
        }
    }
    // 整合
    QString newContent = newLines.join("\n");

    // -----写入-----.
    QFile file(filename);
    // 输出内容
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "file2 filename:" << file.fileName();  // 输出文件名
        QTextStream out(&file);
        out << newContent;
        file.close();
    }
}

