#include "mainui/relpacedialog.h"
#include "ui_relpacedialog.h"

RelpaceDialog::RelpaceDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RelpaceDialog)
{
    ui->setupUi(this);
}

RelpaceDialog::~RelpaceDialog()
{
    delete ui;
}
