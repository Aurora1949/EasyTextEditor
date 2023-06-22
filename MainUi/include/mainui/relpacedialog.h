#ifndef RELPACEDIALOG_H
#define RELPACEDIALOG_H

#include <QWidget>

namespace Ui {
class RelpaceDialog;
}

class RelpaceDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RelpaceDialog(QWidget *parent = nullptr);
    ~RelpaceDialog();

private:
    Ui::RelpaceDialog *ui;
};

#endif // RELPACEDIALOG_H
