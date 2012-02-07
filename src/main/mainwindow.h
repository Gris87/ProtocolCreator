#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QSplitter>

#include "src/widgets/knowncheckbox.h"
#include "src/widgets/pageframe.h"

#include "ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    QSplitter *dividerSplitter;
    PageFrame *contentPage;
    bool errorHappened;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

    void addPage(QString aName, QString aVarName);

    void addError(QString aText);
    void addHint(QString aText);
    void addLog(QString aFullVarName, QString aText);

    void updateHeader();
    void updateAdmin();

    inline void saveState();
    inline void loadState();

public slots:
    void on_actionFindNext_triggered();
    void on_actionFindPrev_triggered();

private slots:
    void pageMoved(int from, int to);
    void page_nameChanged(PageFrame *parentPage);
    void page_useToggled(PageFrame *parentPage);
    void contentCheckBoxToggled(KnownCheckBox *aCheckBox, bool checked);

    void on_pagesTabWidget_tabCloseRequested(int index);

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionGlobalVars_triggered();
    void on_actionSectionControl_triggered();
    void on_actionCheckDocument_triggered();
    void on_actionExport_triggered();
    void on_actionGenerateWord_triggered();
    void on_actionLogin_triggered();
    void on_actionSetDocPass_triggered();
    void on_actionSetAdminPass_triggered();
    void on_actionAddPage_triggered();
    void on_actionVariableInteger_triggered();
    void on_actionVariableString_triggered();
    void on_actionVariableBool_triggered();
    void on_actionVariableDate_triggered();
    void on_actionVariableTime_triggered();
    void on_actionVariableList_triggered();
    void on_actionVariableExtendedList_triggered();
    void on_actionVariableExpression_triggered();
    void on_actionComponentText_triggered();
    void on_actionComponentTable_triggered();
};

#endif // MAINWINDOW_H
