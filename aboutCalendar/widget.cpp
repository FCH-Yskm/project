
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);




    // 获得当前月份
    QDate currentDate = ui->calendarWidget->selectedDate();
    int currentMonth = currentDate.month();
    qDebug() << "当前月份：" << currentMonth;



    // 设置 verticalLayout_2 的对齐方式为顶部对齐
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    ui->verticalLayout->setAlignment(Qt::AlignTop);

    file f;
    QString filename = datapath + "note.csv";
    f.read_matrix(filename,"无");
    for(int i=0;i<f.content_rows_num;i++){
        QLabel *label = new QLabel(f.units[i][0]);
        ui->verticalLayout_2->addWidget(label);
        // 将 label_5 的样式表设置为纯白色背景
        label->setStyleSheet("background-color: white;");
    }

}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_calendarWidget_clicked(const QDate &date)
{
    year = date.year();
    month = date.month();
    day = date.day();

    qDebug() << "日期被点击:" << "年份：" << year<< "月份：" << month << "日期：" << day;
    ui->stackedWidget->setCurrentIndex(1);

    file f;
    QString filename = datapath + "note.csv";
    f.read_matrix(filename,"无");

    for(int i=0;i<f.content_rows_num;i++){
        // 判定
        QStringList temp = f.units[i][0].split("-");
        bool condition1 = temp[0].toInt() == year;
        bool condition2 = temp[1].toInt() == month;
        bool condition3 = temp[2].toInt() == day;
        if(condition1 && condition2 && condition3 ){
            //显示 记事本内容
            ui->textEdit->setText(f.units[i][1]);
            //显示 截止日期
            QStringList temp20 = f.units[i][2].split("丨");
            for(int j=0;j<temp20.size();j++){
                QStringList temp21 = temp20[j].split("-");
                QCheckBox *checkBox = new QCheckBox(temp21[1]);
                if(temp21[0]=="1"){
                    checkBox->setChecked(true);
                }
                ui->verticalLayout->addWidget(checkBox);

            }
            //显示 每日计划
            QStringList temp30 = f.units[i][3].split("丨");
            for(int j=0;j<temp30.size();j++){
                QStringList temp31 = temp30[j].split("-");
                int tempNum = temp31[0].toInt();
                 for(int k=0;k<24;k++){
                    if(tempNum == k){
                    QListWidgetItem *item = ui->listWidget->item(k-1);
                    item->setText(QString::number(k)+"-"+temp31[1]);
                    }
                }
            }

        }
    }
}

// 截止日期 添加
void Widget::on_pushButton_released()
{
    // 显示消息框，并获取用户输入的文本
    bool ok;
    QString text = QInputDialog::getText(nullptr, "输入框", "请输入事项:", QLineEdit::Normal, "", &ok);

    // 如果用户点击了OK按钮
    if (ok && !text.isEmpty()) {
        qDebug() << "用户输入的文本为:" << text;
        QCheckBox *checkBox = new QCheckBox(text);
        ui->verticalLayout->addWidget(checkBox);
    } else {
        qDebug() << "用户取消了输入";
    }
}


// 今日计划 添加
void Widget::on_pushButton_2_released()
{
    QString time;
    QString event;
    for(int i = 0;i<2;i++){
        // 显示消息框，并获取用户输入的文本
        bool ok;
        QString text;
        if(i == 0)text = QInputDialog::getText(nullptr, "输入框", "请输入时间:", QLineEdit::Normal, "", &ok);
        if(i == 1)text = QInputDialog::getText(nullptr, "输入框", "请输入计划:", QLineEdit::Normal, "", &ok);
        // 如果用户点击了OK按钮
        if (ok && !text.isEmpty()) {
            if(i == 0) time = text;
            if(i == 1) event = text;
        } else {
            qDebug() << "用户取消了输入";
        }
    }
    QListWidgetItem *item = ui->listWidget->item(time.toInt()-1);
    item->setText(time +"-"+event);
}

// 保存
void Widget::on_pushButton_3_released()
{
    file f;
    QString filename2 = datapath + "note.csv";
    f.read_matrix(filename2,"无");
// 待办事项界面内容
    QString tempContent;
    QVBoxLayout *layout = ui->verticalLayout;
    // 遍历布局中的子控件，并获取复选框的状态和文本内容
    for (int i = 0; i < layout->count(); ++i) {
        QCheckBox* checkBox = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
        if (checkBox) {
            QString text = checkBox->text();
            QString tempItem;
            bool isChecked = checkBox->isChecked();
            if(isChecked){
                tempItem ="1-" + text;
            }
            else{
                tempItem ="0-" + text;
            }
            tempContent += tempItem;
            if(i<layout->count()-1) tempContent += "丨";
        }
    }

// 计划界面内容
    QString tempContent2;
    int itemCount = ui->listWidget->count();
    for (int i = 0; i < itemCount; ++i) {
        QString tempItem = ui->listWidget->item(i)->text();
        //qDebug() << "项" << i+1 << "的文本内容为:" << tempItem;
        int length = tempItem.length(); // 或者使用 text.size()
        //qDebug() << "项" << i+1 << "的文本长度为:" << length;
        if(length!=1&&length!=2){
            tempContent2 += tempItem;
            if(i<itemCount-2) tempContent2 += "丨";
        }
    }
    if (tempContent2.endsWith("丨")) {
        tempContent2.chop(1);
    }


    bool isExisted = false;
    int i;
    for(i=0;i<f.content_rows_num;i++){

        // 判定
        QStringList temp = f.units[i][0].split("-");
        bool condition1 = temp[0].toInt() == year;
        bool condition2 = temp[1].toInt() == month;
        bool condition3 = temp[2].toInt() == day;


        if(condition1 && condition2 && condition3 ){
            isExisted = 1;
            // 保存 记事本
            f.units[i][1] = ui->textEdit->toPlainText();
            qInfo()<<f.units;

            f.units[i][2] = tempContent;

            f.units[i][3] =tempContent2;
        }
    }
    QStringList newLines;
    if(isExisted==false){

        newLines.append(QString::number(year)+"-"+QString::number(month)+"-"+QString::number(day)) ;
        newLines.append(ui->textEdit->toPlainText());
        newLines.append(tempContent);
        newLines.append(tempContent2);
        f.content_rows_num++;
        f.units.append(newLines);
    }
    //f.units.append(newLines);
    qInfo()<<newLines;


    f.write_matrix(filename2,"无");
}

