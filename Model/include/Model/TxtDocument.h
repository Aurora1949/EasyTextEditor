//
// Created by Jeffrey Hsu on 2023/6/23.
//

#ifndef EASYTEXTEDITOR_TXTDOCUMENT_H
#define EASYTEXTEDITOR_TXTDOCUMENT_H

#include "Model/BaseDocument.h"

class TxtDocument : public BaseDocument {
Q_OBJECT

public:
    explicit TxtDocument(QString &text);

    void clear() override;
};

#endif //EASYTEXTEDITOR_TXTDOCUMENT_H
