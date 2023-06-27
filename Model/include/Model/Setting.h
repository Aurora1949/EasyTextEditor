
#ifndef FONTSETTING_H
#define FONTSETTING_H

#include <QStringList>

class FontSetting
{
public:
    FontSetting();
    FontSetting(QStringList families, int fontStyle, int fontSize);
    ~FontSetting();

public:
    QStringList families;
    int fontStyle{};
    int fontSize{};
};

#endif // FONTSETTING_H
