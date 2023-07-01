#include <MainUi/AboutUi>
#include <SQLIteSupport/SQLite3>
#include <QDateTime>
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->logo->setPixmap(QPixmap(":/logo/resource/logo/logo.png"));
    ui->logo->setScaledContents(true);

    ui->version->setText(
        QString("%1.%2.%3-%4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH).arg(MODE));

    ui->qtVersion->setText(QString("Qt版本：%1").arg(qVersion()));
    ui->sqliteVersion->setText(QString("SQLite版本：%1").arg(CppSQLite3DB::SQLiteVersion()));
    ui->compileTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

About::~About()
{
    delete ui;
}
