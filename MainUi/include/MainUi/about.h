#ifndef ABOUT_H
#define ABOUT_H

#ifndef QT_NO_DEBUG_OUTPUT
#define MODE "debug"
#else
#define MODE "release"
#endif

#include <QWidget>
#include <MainUi/Version>

namespace Ui {
class About;
}

class About : public QWidget
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
