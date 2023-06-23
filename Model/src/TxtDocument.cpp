//
// Created by Jeffrey Hsu on 2023/6/23.
//

#include "Model/TxtDocument.h"

TxtDocument::TxtDocument(QString &text) : BaseDocument(text) {
    extension = ".txt";
}

void TxtDocument::clear() {
    path.clear();
    title = "未命名";
    buffer.clear();
    content.clear();
    extension = ".txt";
    emit changeWindowTitle();
}
