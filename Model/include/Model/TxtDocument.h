//
// Created by Jeffrey Hsu on 2023/6/23.
//

#ifndef EASYTEXTEDITOR_TXTDOCUMENT_H
#define EASYTEXTEDITOR_TXTDOCUMENT_H

#include "Model/AbstractDocument.h"

class TxtDocument : AbstractDocument {
public:
    explicit TxtDocument(string title, string content, string extension);
};


#endif //EASYTEXTEDITOR_TXTDOCUMENT_H
