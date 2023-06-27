
#include <MainUi/MainWindowUi>
#include <MainUi/SQL>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const char *path)
        : QMainWindow(parent), ui(new Ui::MainWindow), path(path) {
    // Init Part
    ui->setupUi(this);
    init();

    // Connect Part
    connect(this, SIGNAL(checkSaveStatus()), document, SLOT(changeSaveStatus()));
    connect(document, SIGNAL(changeWindowTitle()), this, SLOT(onChangeWindowTitle()));

    // Settings Part
    this->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);  // 设置内边距全为0
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setWindowIcon(QIcon(":/logo/resource/logo/logo.png"));
}

MainWindow::~MainWindow() {
    delete ui;
    delete document;
}

void MainWindow::init() {
    // if a file path have been given,
    // call initWithAExistFile. In this part
    // we can make sure document is exist.
    if (path != nullptr)
        initWithAExistFile(path);
    else
        initWithANewTxtFile();
    onChangeWindowTitle();

    // init db
    initSettingDB();

    // add recent history to menu
    this->recentList = new QMenu("最近打开的文件", this);
    ui->menu->addMenu(recentList);
    addHistoryAction();

    // init statusbar
    ui->statusbar->setSizeGripEnabled(false); // 去除状态栏右下角的三角形
    // 文件编码
    QLabel *encodeText = new QLabel(this);
    encodeText->setText(document->getAnEncode());
    ui->statusbar->addPermanentWidget(encodeText);
    // 指针位置
}

void MainWindow::initWithANewTxtFile() {
    document = new TxtDocument(fileCache);
}

void MainWindow::initWithAExistFile(const char *filePath) {
    document = new BaseDocument(fileCache);
    document->setDocument(filePath);
    QString ext = document->getExtension();
    delete document;
    if (ext == "txt") {
        document = new TxtDocument(fileCache);
    } else if (ext == "c" || ext == "h" || ext == "cpp" || ext == "hpp") {
        document = new BaseDocument(fileCache);
    } else {
        document = new BaseDocument(fileCache);
    }
    openFile(filePath, true);
}

void MainWindow::initSettingDB() {
    QFile dbFile(DATABASE_LOCATION);
    CppSQLite3DB db;
    // check if db exists.
    // if exist then we load settings from it,
    // else we create a new one.
    if (dbFile.open(QIODevice::ReadOnly)) {
        dbFile.close();
        try {
            db.open(DATABASE_LOCATION);
            CppSQLite3Query query = db.execQuery(LOAD_SETTINGS);
            fs.families = QString(query.getStringField(2)).split(',');
            fs.fontSize = query.getIntField(0);
            fs.fontStyle = query.getIntField(1);
            ui->plainTextEdit->setFont(getQFontFromFontSetting(fs));
            query.finalize();
            db.close();
        } catch (CppSQLite3Exception &e) {
            QMessageBox::critical(this, "DB Failed", e.errorMessage());
        }
    } else {
        try {
            db.open(DATABASE_LOCATION);
            db.execDML(CREATE_TABLE_SETTINGS);
            db.execDML(CREATE_TABLE_HISTORY);
            db.execDML(SET_INIT_SETTINGS);
            db.close();
        } catch (CppSQLite3Exception &exception) {
            QMessageBox::critical(this, "DB Failed", exception.errorMessage());
        }
    }
}

void MainWindow::addHistoryAction() {
    CppSQLite3DB db;
    try {
        db.open(DATABASE_LOCATION);
        CppSQLite3Query q = db.execQuery(LOAD_ALL_HISTORY);
        QAction *action;
        while (!q.eof()) {
            action = new QAction(q.getStringField(0));
            qDebug() << "Action" << action;
            recentList->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onHistoryClicked()));
            q.nextRow();
        }
        q.finalize();
        db.close();
    } catch (CppSQLite3Exception &e) {
        QMessageBox::critical(this, "DB Failed", e.errorMessage());
    }
}

void MainWindow::onHistoryClicked() {
    QAction *action = qobject_cast<QAction *>(sender());
    openFile(action->text().toStdString().c_str());
}

void MainWindow::saveSettings() {
    CppSQLite3DB db;
    try {
        db.open(DATABASE_LOCATION);
        CppSQLite3Statement stmt = db.compileStatement(SET_SETTINGS);
        QFont font = ui->plainTextEdit->font();
        stmt.bind(1, font.pointSize());
        int value = font.bold() ? (font.italic() ? 3 : 2) : font.italic() ? 1 : 0;
        stmt.bind(2, value);
        stmt.bind(3, font.families().join(',').toStdString().c_str());
        stmt.execDML();
        stmt.finalize();
        db.close();
    } catch (CppSQLite3Exception &e) {
        // DO NOTHING HERE
        db.close();
    }
}

void MainWindow::onChangeWindowTitle() {
    setWindowTitle((document->isSaved() ? "" : "● ") + document->getTitle()
                   + document->getExtension() + " - 简易记事本");
}

void MainWindow::updateCache(QString cache) {
    this->fileCache = cache;
    emit checkSaveStatus();
}

void MainWindow::on_action_open_triggered() {
    qDebug() << "Open File Button Clicked";
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", "", "文本文件 (*.txt)");
    if (filename.isEmpty())
        return;
    openFile(filename.toStdString().c_str());
}

void MainWindow::openFile(const char *filePath, bool useLocal) {
    QString u8filePath;
    // if filePath contain chinese we change encoding to gbk
    if (useLocal)
        u8filePath = QString::fromLocal8Bit(filePath);
    else
        u8filePath = QString(filePath);
    // if path contain '\' we replace it to '/'
    u8filePath.replace('\\', '/');

    addHistory(u8filePath.toStdString().c_str());
    this->document->setDocument(u8filePath);
    this->ui->plainTextEdit->setPlainText(document->getContent());
    this->ui->statusbar->showMessage("当前打开文件: " + document->getPath());
}

void MainWindow::addHistory(const char *filePath) {
    CppSQLite3DB db;
    try {
        db.open(DATABASE_LOCATION);
        CppSQLite3Statement stmt = db.compileStatement(INSERT_NEW_HISTORY);
        stmt.bind(1, filePath);
        stmt.execDML();
        stmt.finalize();
        db.close();
    } catch (CppSQLite3Exception &e) {
        QMessageBox::critical(this, "DB Failed", e.errorMessage());
    }
}

void MainWindow::on_action_save_triggered() {
    qDebug() << "Save File Button Clicked";

    if (document->isSaved()) {
        this->ui->statusbar->showMessage("文件没有被修改", 5000);
        return;
    }

    // 当为空文件时
    if (document->isEmpty()) {
        QString filename = QFileDialog::getSaveFileName(this, "储存为", "", "文本文件 (*.txt)");
        if (filename.isEmpty())
            return;
        qDebug() << "New File Name is" << filename;
        document->setDocument(filename);
    }
    document->save();
    if (document->write()) {
        this->ui->statusbar->showMessage("保存成功", 5000);
    }
}

void MainWindow::on_plainTextEdit_textChanged() {
    this->undoStack.push(new QUndoCommand(this->ui->plainTextEdit->toPlainText()));
    updateCache(ui->plainTextEdit->toPlainText());
}

void MainWindow::on_action_save_as_triggered() {
    qDebug() << "Save File As Button Clicked";
    QString filename = QFileDialog::getSaveFileName(this, "另存为", "", "文本文件 (*.txt)");

    if (filename.isEmpty())
        return;

    document->setPath(filename);
    document->save();
    document->write();
    onChangeWindowTitle();
}

void MainWindow::on_action_printer_triggered() {
    qDebug() << "Print Button Clicked";
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() == QDialog::Accepted) {
        QTextDocument doc;
        document->save();
        document->write();
        doc.setPlainText(document->getBuffer());
        doc.print(&printer);
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
    if (!document->isSaved()) {
        QMessageBox::StandardButton saveCheck
                = QMessageBox::question(this,
                                        "未保存",
                                        "此文件尚未保存，是否将更改保存到“" + document->getTitle()
                                        + document->getExtension() + "”中？",
                                        QMessageBox::Save | QMessageBox::Cancel,
                                        QMessageBox::Save);
        if (saveCheck == QMessageBox::Save) {
            this->on_action_save_triggered();
        }
    }
    ui->plainTextEdit->clear();
    document->clear();
    updateCache(ui->plainTextEdit->toPlainText());
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
    this->activateWindow();
    if (text.isEmpty()) {
        return;
    }
    if (!findNextMatch(this->ui->plainTextEdit, text, isWord, isCap)) {
        QMessageBox::information(this, "提示", "未找到匹配的文本。");
        return;
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

void MainWindow::closeEvent(QCloseEvent *ev) {
    if (!document->isSaved()) {
        QMessageBox msgBox(QMessageBox::Warning,
                           "关闭",
                           "此文件尚未保存，是否将更改保存到“" + document->getTitle() + document->getExtension() +
                           "”中？");
        QPushButton *saveButton = msgBox.addButton(tr("保存"), QMessageBox::AcceptRole);
        QPushButton *discardButton = msgBox.addButton(tr("不保存"), QMessageBox::DestructiveRole);
        QPushButton *cancelButton = msgBox.addButton(tr("取消"), QMessageBox::RejectRole);

        int ret = msgBox.exec();

        if (msgBox.clickedButton() == saveButton) {
            // 执行保存操作
            qDebug("Saving...");
            on_action_save_triggered();
        } else if (msgBox.clickedButton() == discardButton) {
            // 不保存，直接关闭窗口
            qDebug("Discarding...");
        } else {
            // 取消关闭操作
            ev->ignore();
        }
        return;
    }
    saveSettings();
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
                         bool isCap) {
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

void MainWindow::showErrorMsg(QString msg) {
    QMessageBox::critical(this, "错误", msg);
}

void MainWindow::on_action_settings_triggered() {
    SettingDialog *sd = new SettingDialog(fs);
    sd->setAttribute(Qt::WA_DeleteOnClose);
    sd->setAttribute(Qt::WA_QuitOnClose, false);
    connect(sd, SIGNAL(submitFontSetting(FontSetting)), this, SLOT(acceptFontSetting(FontSetting)));
    sd->show();
}

QFont MainWindow::getQFontFromFontSetting(const FontSetting &fs) {
    QStringList lst = fs.families;
    QFont font(lst);
    int fontStyle = fs.fontStyle;
    int fontSize = fs.fontSize;
    switch (fontStyle) {
        case 1:
            font.setBold(true);
            break;
        case 2:
            font.setItalic(true);
            break;
        case 3:
            font.setBold(true);
            font.setItalic(true);
            break;
        default:
            break;
    }
    font.setPointSize(fontSize);
    return font;
}

void MainWindow::acceptFontSetting(const FontSetting &fs) {
    QFont font = getQFontFromFontSetting(this->fs);
    ui->plainTextEdit->setFont(font);
}
