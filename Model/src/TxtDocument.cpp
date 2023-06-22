//
// Created by Jeffrey Hsu on 2023/6/23.
//

#include "Model/TxtDocument.h"

TxtDocument::TxtDocument(string title, string content, string extension) {
    this->setTitle(title);
    this->setContent(content);
    this->setExtension(extension);
}
