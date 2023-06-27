#include "ui_setting.h"
#include "Model/Setting.h"
#include "MainUi/setting.h"


#include <MainUi/SettingUi>

SettingDialog::SettingDialog(FontSetting &fs, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingDialog), fs(fs)
{
    ui->setupUi(this);

    ui->englishFontComboBox->setCurrentFont(QFont(fs.families.first()));
    ui->chineseFontComboBox->setCurrentFont(QFont(fs.families.last()));
    ui->fontStyleComboBox->setCurrentIndex(fs.fontStyle);
    ui->fontStyleLineEdit->setText(ui->fontStyleComboBox->currentText());
    ui->fontSizeLineEdit->setText(QString("%1").arg(fs.fontSize));

    ui->plainTextEdit->setFont(QFont(fs.families));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_comfirmPushButton_clicked()
{
    QStringList lst;
    lst.append(ui->englishFontComboBox->currentFont().family());
    lst.append(ui->chineseFontComboBox->currentFont().family());
    // FontSetting fs(lst, ui->fontStyleComboBox->currentIndex(), ui->fontSizeLineEdit->text().toInt());
    fs.families = lst;
    fs.fontStyle = ui->fontStyleComboBox->currentIndex();
    fs.fontSize = ui->fontSizeLineEdit->text().toInt();
    emit submitFontSetting(fs);
    this->close();
}

void SettingDialog::on_cancelPushButton_clicked() {
    this->close();
}

void SettingDialog::on_defaultPushButton_clicked() {
    return;
}

void SettingDialog::on_chineseFontComboBox_currentFontChanged(QFont f) {
    QStringList lst = ui->plainTextEdit->font().families();
    lst.last() = f.family();
    ui->plainTextEdit->setFont(QFont(lst));
}

void SettingDialog::on_englishFontComboBox_currentFontChanged(QFont f) {
    QStringList lst = ui->plainTextEdit->font().families();
    lst.first() = f.family();
    ui->plainTextEdit->setFont(QFont(lst));
}
