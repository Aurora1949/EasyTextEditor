
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef QT_NO_DEBUG_OUTPUT
// change db file to your source dir if you want.
#define DATABASE_LOCATION "../../data.db"
#else
#define DATABASE_LOCATION "D:\\TextEditor\\data.db"
#endif

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QtPrintSupport>
#include <QStringList>

#include <MainUi/AboutUi>
#include <MainUi/ReplaceUi>
#include <MainUi/SearchUi>
#include <MainUi/SettingUi>

#include <Model/BaseDocument.h>
#include <Model/TxtDocument.h>
#include <Model/Setting.h>

#include <SQLIteSupport/SQLite3>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, const char *path = nullptr);

    ~MainWindow();

signals:

    void checkSaveStatus();

private slots:

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_plainTextEdit_textChanged();

    void on_action_save_as_triggered();

    void on_action_printer_triggered();

    void on_action_cut_triggered();

    void on_action_copy_triggered();

    void on_action_paste_triggered();

    void on_action_new_triggered();

    void on_action_close_triggered();

    void on_action_undo_triggered();

    void on_action_redo_triggered();

    void on_action_search_triggered();

    void onSearchClicked(const QString &, bool, bool);

    void onReplaceClicked(const QString &, const QString &, bool, bool, int);

    void on_action_substitution_triggered();

    void on_action_amplify_triggered();

    void on_action_reduce_triggered();

    void on_action_reset_scale_triggered();

    void on_action_about_triggered();

    void onChangeWindowTitle();

    void onHistoryClicked();

    void on_action_settings_triggered();

    void acceptFontSetting(const FontSetting &);

private:
    Ui::MainWindow *ui;
    QString currFile;
    QString filename;
    QString fileCache;
    const char *path;
    BaseDocument *document;
    QUndoStack undoStack;
    QMenu *recentList;
    FontSetting fs;
    //CppSQLite3DB db;

    bool isSaved;

    void updateCache(QString cache);

    bool findNextMatch(QPlainTextEdit *, const QString &, bool, bool);

    bool replace(QPlainTextEdit *, const QString &, const QString &, bool, bool);

    void showErrorMsg(QString);

    void closeEvent(QCloseEvent *) override;

    void openFile(const char *filePath, bool useLocal = false);

    void initWithANewTxtFile();

    void initWithAExistFile(const char *filePath);

    void init();

    void initSettingDB();

    void saveSettings();

    void addHistoryAction();

    void addHistory();

    void addHistory(const char *filePath);

    QFont getQFontFromFontSetting(const FontSetting &fs);
};

#endif // MAINWINDOW_H
