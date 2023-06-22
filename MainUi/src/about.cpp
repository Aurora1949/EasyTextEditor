#include <mainui/about.h>
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
    ui->compileTime->setText(__TIMESTAMP__);
}

About::~About()
{
    delete ui;
}
