#ifndef REPLACE_H
#define REPLACE_H

#include <QCloseEvent>
#include <QDebug>
#include <QWidget>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

signals:
    void replaceClicked(const QString &, const QString &, bool, bool, int);
    void searchClicked(const QString, bool, bool);

private slots:
    void on_replaceOnce_clicked();

    void on_replaceAll_clicked();

    void on_findNext_clicked();

private:
    Ui::ReplaceDialog *ui;
};

#endif // REPLACE_H
