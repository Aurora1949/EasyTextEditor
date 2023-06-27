#include <MainUi/SearchUi>
#include "./ui_search.h"

SearchDialog::SearchDialog(QWidget *parent)
    : QWidget(parent, Qt::Widget | Qt::WindowStaysOnTopHint)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_nextButton_clicked()
{
    emit searchClicked(this->ui->content->text(),
                       this->ui->checkBox->isChecked(),
                       this->ui->checkBox_2->isChecked());
}

void SearchDialog::closeEvent(QCloseEvent *ev)
{
    qDebug() << "Search Window Close";
    ev->accept();
}

void SearchDialog::on_cancelButton_clicked()
{
    this->close();
}
