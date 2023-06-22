//
// Created by Jeffrey Hsu on 2023/6/23.
//

#ifndef EASYTEXTEDITOR_ABSTRACTDOCUMENT_H
#define EASYTEXTEDITOR_ABSTRACTDOCUMENT_H

#include <iostream>

#include <string>

using namespace std;

class AbstractDocument {
public:
    explicit AbstractDocument();

    ~AbstractDocument();

    const string &getTitle() const;

    void setTitle(const string &title);

    const string &getContent() const;

    void setContent(const string &content);

    const string &getExtension() const;

    void setExtension(const string &extension);

private:
    string title;
    string content;
    string extension;
    string buff;
public:
    const string &getBuff() const;

    void setBuff(const string &buff);
};

#endif //EASYTEXTEDITOR_ABSTRACTDOCUMENT_H
