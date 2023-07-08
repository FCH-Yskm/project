
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>

#include <QApplication>
// checkbox
#include <QCheckBox>
#include <QVBoxLayout>
// 对话框
#include <QInputDialog>

#include "file.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_released();

    void on_pushButton_2_released();

    void on_pushButton_3_released();

private:
    Ui::Widget *ui;
    int year;
    int month;
    int day;
    QString datapath = "D:/aboutCalendar/aboutCalendar/aboutCalendar/content/";


};

#endif // WIDGET_H
