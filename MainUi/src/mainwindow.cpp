
#include <mainui/mainwindow.h>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    this->setWindowIcon(QIcon(":/logo/logo.png"));
    this->clipboard = QApplication::clipboard();
    this->filename = "未命名";
    this->isSaved = true;
    this->setAttribute(Qt::WA_QuitOnClose, true);
    setWindowTitle("未命名 - 简易记事本");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setTitle(QString filename) {
    this->filename = filename.split('/').last().split('.').front();
    this->currFile = filename;
    this->setWindowTitle(this->filename + " - 简易记事本");
}

void MainWindow::setTitleToNotSave() {
    this->setWindowTitle("● " + (this->filename.isEmpty() ? "未命名" : this->filename)
                         + " - 简易记事本");
    this->isSaved = false;
}

void MainWindow::setTitleToNormal() {
    this->setWindowTitle((this->filename.isEmpty() ? "未命名" : this->filename) + " - 简易记事本");
    this->isSaved = true;
}

void MainWindow::updateCache(QString cache) {
    this->fileCache = cache;
}

void MainWindow::on_action_open_triggered() {
    qDebug() << "Open File Button Clicked";
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", "", "文本文件 (*.txt)");
    if (filename.isEmpty())
        return;

    qDebug() << "File Name is" << filename;
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "文件打开失败", "文件打开失败，请确保你有权限访问此文件。");
        return;
    }

    qDebug() << this->filename;
    this->setTitle(filename);

    QTextStream text(&file);
    this->ui->statusbar->showMessage("当前打开文件: " + filename);

    this->updateCache("");
    this->ui->plainTextEdit->clear();

    this->updateCache(text.readAll());
    this->ui->plainTextEdit->setPlainText(this->fileCache);

    file.close();
}

void MainWindow::on_action_save_triggered() {
    qDebug() << "Save File Button Clicked";

    if (!this->currFile.isEmpty() && (this->fileCache == this->ui->plainTextEdit->toPlainText())) {
        this->ui->statusbar->showMessage("文件没有被修改", 5000);
        return;
    }

    QFile file;
    if (this->currFile.isEmpty()) {
        // 当文件为新文件时
        QString filename = QFileDialog::getSaveFileName(this, "储存为", "", "文本文件 (*.txt)");
        if (filename.isEmpty())
            return;
        qDebug() << "New File Name is" << filename;
        this->setTitle(filename);
        file.setFileName(filename);
    } else {
        file.setFileName(this->currFile);
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "文件保存失败", "文件保存失败，请确保你有权限访问此文件。");
        return;
    }

    file.write(this->ui->plainTextEdit->toPlainText().toStdString().c_str());
    this->updateCache(this->ui->plainTextEdit->toPlainText());
    this->ui->statusbar->showMessage("保存成功", 5000);
    this->setTitleToNormal();

    file.close();
}

void MainWindow::on_plainTextEdit_textChanged() {
    this->undoStack.push(new QUndoCommand(this->ui->plainTextEdit->toPlainText()));
    if (this->fileCache == this->ui->plainTextEdit->toPlainText()) {
        this->setTitleToNormal();
        return;
    }
    this->setTitleToNotSave();
}

void MainWindow::on_action_save_as_triggered() {
    qDebug() << "Save File As Button Clicked";
    QString filename = QFileDialog::getSaveFileName(this, "另存为", "", "文本文件 (*.txt)");

    if (filename.isEmpty())
        return;

    qDebug() << "New File Name is" << filename;
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "文件保存失败", "文件保存失败，请确保你有权限访问此文件。");
        return;
    }

    this->setTitle(filename);
    this->currFile = filename;

    file.write(this->ui->plainTextEdit->toPlainText().toStdString().c_str());
    this->updateCache(this->ui->plainTextEdit->toPlainText());
    this->setTitleToNormal();

    file.close();
}

void MainWindow::on_action_printer_triggered() {
    qDebug() << "Print Button Clicked";
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog * dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() == QDialog::Accepted) {
        QTextDocument document;
        document.setPlainText(this->ui->plainTextEdit->toPlainText());
        document.print(&printer);
    }
}

void MainWindow::on_action_cut_triggered() {
    qDebug() << "Cut Button Clicked";
    this->ui->plainTextEdit->cut();
}

void MainWindow::on_action_copy_triggered() {
    qDebug() << "Copy Button Clicked";
    this->ui->plainTextEdit->copy();
}

void MainWindow::on_action_paste_triggered() {
    qDebug() << "Paste Button Clicked";
    this->ui->plainTextEdit->paste();
}

void MainWindow::on_action_new_triggered() {
    qDebug() << "New File Button Clicked";
    if (!isSaved) {
        QMessageBox::StandardButton saveCheck
                = QMessageBox::question(this,
                                        "未保存",
                                        "此文件尚未保存，是否将更改保存到“" + this->filename
                                        + ".txt”中？",
                                        QMessageBox::Save | QMessageBox::Cancel,
                                        QMessageBox::Save);
        if (saveCheck == QMessageBox::Save) {
            this->on_action_save_triggered();
        }
    }
    this->updateCache("");
    this->ui->plainTextEdit->clear();
    this->setTitle("未命名.txt");
    this->currFile = "";
    this->filename = "未命名";
}

void MainWindow::on_action_close_triggered() {
    qDebug() << "Close Button Clicked";
    this->on_action_new_triggered();
}

void MainWindow::on_action_undo_triggered() {
    qDebug() << "Undo Button Clicked";
    this->ui->plainTextEdit->undo();
}

void MainWindow::on_action_redo_triggered() {
    qDebug() << "Redo Button Clicked";
    this->ui->plainTextEdit->redo();
}

void MainWindow::on_action_search_triggered() {
    qDebug() << "Search Button Clicked";
    SearchDialog *search = new SearchDialog();
    search->setAttribute(Qt::WA_DeleteOnClose); // 在SearchDialog关闭时自动释放内存
    search->setAttribute(Qt::WA_QuitOnClose, false);
    search->show();
    connect(search,
            SIGNAL(searchClicked(
    const QString &, bool, bool)),
    this,
            SLOT(onSearchClicked(
    const QString &, bool, bool)));
}

void MainWindow::onSearchClicked(const QString &text, bool isWord, bool isCap) {
    if (!findNextMatch(this->ui->plainTextEdit, text, isWord, isCap)) {
        QMessageBox::information(this, "提示", "未找到匹配的文本。");
    }
}

bool MainWindow::findNextMatch(QPlainTextEdit *plainTextEdit,
                               const QString &searchText,
                               bool wholeWords,
                               bool caseSensitivity) {
    QTextCursor cursor = plainTextEdit->textCursor(); // 获取当前光标位置
    QTextDocument::FindFlags flags;
    if (wholeWords) {
        qDebug() << "Find Whole Words ON";
        flags |= QTextDocument::FindWholeWords;
    }

    if (caseSensitivity) {
        qDebug() << "Find Case Sensitively ON";
        flags |= QTextDocument::FindCaseSensitively;
    }

    if (cursor.hasSelection()) { // 如之前有匹配，取消选中
        cursor.clearSelection();
    }

    return plainTextEdit->find(searchText, flags); // 查找下一个匹配的文本
}

void MainWindow::searchPlainTextEdit(QPlainTextEdit *textEdit, const QString &keyword) {
    QTextCursor cursor(textEdit->document());

    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = textEdit->document()->find(keyword, cursor);
        if (!cursor.isNull()) {
            cursor.select(QTextCursor::WordUnderCursor);
            if (cursor.selectedText() == keyword) {
                QTextCharFormat format;
                format.setBackground(Qt::yellow);
                cursor.mergeCharFormat(format);
            }
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *ev) {
    if (!isSaved) {
        QMessageBox msgBox(QMessageBox::Warning,
                           "关闭",
                           "此文件尚未保存，是否将更改保存到“" + this->filename + ".txt”中？");
        QPushButton *saveButton = msgBox.addButton(tr("保存"), QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton(tr("不保存"), QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton(tr("取消"), QMessageBox::RejectRole);

        int ret = msgBox.exec();

        if (msgBox.clickedButton() == saveButton) {
            // 执行保存操作
            qDebug("Saving...");
            on_action_save_triggered();
            ev->accept();
        } else if (msgBox.clickedButton() == discardButton) {
            // 不保存，直接关闭窗口
            qDebug("Discarding...");
            ev->accept();
        } else {
            // 取消关闭操作
            ev->ignore();
        }
        return;
    }
    ev->accept();
}

void MainWindow::on_action_substitution_triggered() {
    ReplaceDialog *d = new ReplaceDialog();
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->setAttribute(Qt::WA_QuitOnClose, false);
    d->show();
    connect(d,
            SIGNAL(replaceClicked(
    const QString &, const QString &, bool, bool, int)),
    this,
            SLOT(onReplaceClicked(
    const QString &, const QString &, bool, bool, int)));
    connect(d,
            SIGNAL(searchClicked(
    const QString &, bool, bool)),
    this,
            SLOT(onSearchClicked(
    const QString &, bool, bool)));
}

void MainWindow::onReplaceClicked(
        const QString &reg, const QString &text, bool isWords, bool isCap, int type = 0) {
    qDebug() << "Replace Start. Type" << type;
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    bool flag = false;
    do {
        flag = replace(ui->plainTextEdit, reg, text, isWords, isCap);
    } while (flag && type);

    if (type) {
        QMessageBox::information(this, "替换", "全部替换完成");
    } else if (!type && !flag) {
        QMessageBox::information(this, "替换", "没有更多的匹配文本了。");
    }
}

bool MainWindow::replace(QPlainTextEdit *plainTextEdit,
                         const QString &reg,
                         const QString &text,
                         bool isWords,
                         bool isCap)
{
    QTextCursor cursor;
    if (findNextMatch(plainTextEdit, reg, isWords, isCap)) {
        cursor = plainTextEdit->textCursor();
        cursor.beginEditBlock();
        if (cursor.hasSelection()) {
            qDebug() << "Selected Text is: " << cursor.selectedText();
            cursor.removeSelectedText();
            cursor.insertText(text);
        }
        cursor.endEditBlock();
        return true;
    }
    return false;
}

void MainWindow::on_action_amplify_triggered() {
    QFont font = ui->plainTextEdit->font();
    int fontSize = font.pointSize();
    font.setPointSize(++fontSize);
    ui->plainTextEdit->setFont(font);
}

void MainWindow::on_action_reduce_triggered() {
    QFont font = ui->plainTextEdit->font();
    int fontSize = font.pointSize();
    font.setPointSize(--fontSize);
    ui->plainTextEdit->setFont(font);
}

void MainWindow::on_action_reset_scale_triggered() {
    QFont font = ui->plainTextEdit->font();
    int fontSize = 9;
    font.setPointSize(fontSize);
    ui->plainTextEdit->setFont(font);
}

void MainWindow::on_action_about_triggered() {
    About *a = new About();
    a->setAttribute(Qt::WA_DeleteOnClose);
    a->setAttribute(Qt::WA_QuitOnClose, false);
    a->show();
}
