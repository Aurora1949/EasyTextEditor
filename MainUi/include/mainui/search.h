#ifndef SEARCH_H
#define SEARCH_H

#include <QCloseEvent>
#include <QWidget>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog();

signals:
    void searchClicked(const QString, bool, bool);

private slots:
    void on_nextButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::SearchDialog *ui;
    void closeEvent(QCloseEvent *ev) override;
};

#endif // SEARCH_H
