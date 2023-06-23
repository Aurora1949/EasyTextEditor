//
// Created by Jeffrey Hsu on 2023/6/23.
//

#ifndef EASYTEXTEDITOR_BASEDOCUMENT_H
#define EASYTEXTEDITOR_BASEDOCUMENT_H

#include <QFile>
#include <QTextStream>

#include <Model/AbstractDocument.h>

class BaseDocument : public AbstractDocument {
Q_OBJECT

public:
    explicit BaseDocument(QString &ref);

    ~BaseDocument();

protected:
    QString path;
    QString title;
    QString buffer;
    QString encode;
    QString &content;
    QString extension;

    bool saveStatus;

public slots:
    void changeSaveStatus();

signals:
    void changeWindowTitle();

public:
    const QString &getPath() const;

    void setPath(const QString &path);

    const QString &getTitle() const;

    void setTitle(const QString &title);

    const QString &getBuffer() const;

    void setBuffer(const QString &buffer);

    QString &getContent() const;

    void setContent(QString &content);

    const QString &getExtension() const;

    void setExtension(const QString &extension);

    const QString &getAnEncode() const;

    void setAnEncode(const QString &anEncode);

    bool isSaved() const;

    void setSaveStatus(bool saveStatus);

    virtual void save();

    virtual void clear();

    virtual bool isEmpty();

    virtual void setDocument(QString path);

    virtual bool write();
};


#endif //EASYTEXTEDITOR_BASEDOCUMENT_H
