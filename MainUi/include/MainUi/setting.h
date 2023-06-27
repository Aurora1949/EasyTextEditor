#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <Model/Setting.h>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SettingDialog(FontSetting &fs, QWidget *parent = nullptr);
    ~SettingDialog();

signals:
    void submitFontSetting(FontSetting);

private slots:
    void on_comfirmPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_defaultPushButton_clicked();

    void on_chineseFontComboBox_currentFontChanged(QFont);

    void on_englishFontComboBox_currentFontChanged(QFont);

private:
    Ui::SettingDialog *ui;
    FontSetting &fs;
};

#endif // SETTING_H
