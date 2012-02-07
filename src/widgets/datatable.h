#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableWidget>

#include <QTimer>

class DataTable : public QTableWidget
{
    Q_OBJECT
public:
    QWidget *mTable;

    explicit DataTable(QWidget *parent = 0);

    QModelIndex indexFromItem(QTableWidgetItem *item) const;

    void pasteData();

protected:
    void commitData(QWidget *editor);
    void keyPressEvent(QKeyEvent *event);

private:
    int lastRow;
    int lastCol;

private slots:
    void jumpToNext();
};

#endif // DATATABLE_H
