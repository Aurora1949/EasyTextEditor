// Created by Jeffrey Hsu on 2023/6/23.
//
//
#include "Model/AbstractDocument.h"

AbstractDocument::AbstractDocument() = default;

AbstractDocument::~AbstractDocument() = default;

const string &AbstractDocument::getTitle() const {
    return title;
}

void AbstractDocument::setTitle(const string &title) {
    AbstractDocument::title = title;
}

const string &AbstractDocument::getContent() const {
    return content;
}

void AbstractDocument::setContent(const string &content) {
    AbstractDocument::content = content;
}

const string &AbstractDocument::getExtension() const {
    return extension;
}

void AbstractDocument::setExtension(const string &extension) {
    AbstractDocument::extension = extension;
}

const string &AbstractDocument::getBuff() const {
    return buff;
}

void AbstractDocument::setBuff(const string &buff) {
    AbstractDocument::buff = buff;
}
