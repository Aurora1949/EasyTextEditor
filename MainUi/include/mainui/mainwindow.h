
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QtPrintSupport>
#include <Model/TxtDocument.h>

#include "about.h"
#include "replace.h"
#include "search.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
    QString currFile;
    QString filename;
    QString fileCache;
    QClipboard *clipboard;
    QUndoStack undoStack;
    bool isSaved;

    void setTitle(QString);

    void updateCache(QString);

    void setTitleToNotSave();

    void setTitleToNormal();

    void saveFile();

    void searchPlainTextEdit(QPlainTextEdit *, const QString &);

    void closeEvent(QCloseEvent *) override;

    bool findNextMatch(QPlainTextEdit *, const QString &, bool, bool);

    bool replace(QPlainTextEdit *, const QString &, const QString &, bool, bool);
};

#endif // MAINWINDOW_H
