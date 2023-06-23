//
// Created by Jeffrey Hsu on 2023/6/23.
//

#include "Model/BaseDocument.h"

BaseDocument::BaseDocument(QString &ref) : content(ref) {
    title = "未命名";
    saveStatus = true;
}

BaseDocument::~BaseDocument() {

}

const QString &BaseDocument::getPath() const {
    return path;
}

void BaseDocument::setPath(const QString &path) {
    BaseDocument::path = path;
}

const QString &BaseDocument::getTitle() const {
    return title;
}

void BaseDocument::setTitle(const QString &title) {
    BaseDocument::title = title;
}

const QString &BaseDocument::getBuffer() const {
    return buffer;
}

void BaseDocument::setBuffer(const QString &buffer) {
    BaseDocument::buffer = buffer;
}

const QString &BaseDocument::getExtension() const {
    return extension;
}

void BaseDocument::setExtension(const QString &extension) {
    BaseDocument::extension = extension;
}

QString &BaseDocument::getContent() const {
    return content;
}

void BaseDocument::setContent(QString &content) {
    BaseDocument::content = content;
}

const QString &BaseDocument::getAnEncode() const {
    return encode;
}

void BaseDocument::setAnEncode(const QString &anEncode) {
    encode = anEncode;
}

bool BaseDocument::isEmpty() {
    return path.isEmpty();
}

void BaseDocument::clear() {
    path.clear();
    title = "未命名";
    buffer.clear();
    content.clear();
    extension.clear();
    emit changeWindowTitle();
}

void BaseDocument::save() {
    buffer = content;
    changeSaveStatus();
}

void BaseDocument::setDocument(QString path) {
    this->path = path;
    QString file = path.split('/').last();
    this->title = file.split('.').first();
    this->extension = "." + file.split('.').last();

    QFile f(path);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);
        this->content = this->buffer = in.readAll();
        this->encode = QStringConverter::nameForEncoding(in.encoding());
        f.close();
    } else {
        // TODO: 使用槽发送信号
    }
}

bool BaseDocument::write() {
    QFile f(path);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        f.write(buffer.toStdString().c_str());
        f.close();
    } else {
        // TODO: 使用槽函数发送信号
        return false;
    }
    return true;
}

void BaseDocument::changeSaveStatus() {
    saveStatus = buffer == content;
    emit changeWindowTitle();
}

bool BaseDocument::isSaved() const {
    return saveStatus;
}

void BaseDocument::setSaveStatus(bool saveStatus) {
    BaseDocument::saveStatus = saveStatus;
}
