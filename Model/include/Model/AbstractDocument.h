//
// Created by Jeffrey Hsu on 2023/6/23.
//

#ifndef EASYTEXTEDITOR_ABSTRACTDOCUMENT_H
#define EASYTEXTEDITOR_ABSTRACTDOCUMENT_H

#include <QObject>

class AbstractDocument : public QObject {
Q_OBJECT

public:
    explicit AbstractDocument(QObject *parent = nullptr);
};

#endif //EASYTEXTEDITOR_ABSTRACTDOCUMENT_H
