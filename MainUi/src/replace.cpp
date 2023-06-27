#include <MainUi/ReplaceUi>
#include "ui_replace.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QWidget(parent, Qt::WindowStaysOnTopHint | Qt::Widget),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_replaceOnce_clicked()
{
    emit replaceClicked(ui->content->text(),
                        ui->replaceContent->text(),
                        ui->checkBox->isChecked(),
                        ui->checkBox_2->isChecked(),
                        0);
}

void ReplaceDialog::on_replaceAll_clicked()
{
    emit replaceClicked(ui->content->text(),
                        ui->replaceContent->text(),
                        ui->checkBox->isChecked(),
                        ui->checkBox_2->isChecked(),
                        1);
}

void ReplaceDialog::on_findNext_clicked()
{
    emit searchClicked(ui->content->text(), ui->checkBox->isChecked(), ui->checkBox_2->isChecked());
}
