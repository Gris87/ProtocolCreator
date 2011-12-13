#ifndef GLOBALDIALOG_H
#define GLOBALDIALOG_H

#include <QDialog>

#include "ui_globaldialog.h"

#include "src/components/pagecomponent.h"

namespace Ui {
    class GlobalDialog;
}

class GlobalDialog : public QDialog
{
    Q_OBJECT

public:
    Ui::GlobalDialog *ui;
    QList<PageComponent*> variables;

    explicit GlobalDialog(QWidget *parent = 0);
    ~GlobalDialog();

    void addVariable(PageComponent* aComponent);

    void saveToStream(QDataStream &aStream);
    void loadFromStream(QDataStream &aStream);

    void updateAdmin();
private slots:
    void on_numberButton_clicked();
    void on_stringButton_clicked();
    void on_boolButton_clicked();
    void on_dateButton_clicked();
    void on_timeButton_clicked();
    void on_listButton_clicked();
    void on_extendedListButton_clicked();
    void on_expressionButton_clicked();
};

#endif // GLOBALDIALOG_H
