#ifndef DATATABLE_H
#define DATATABLE_H

#include <QTableWidget>

class DataTable : public QTableWidget
{
    Q_OBJECT
public:
    QWidget *mTable;

    explicit DataTable(QWidget *parent = 0);

    QModelIndex indexFromItem(QTableWidgetItem *item) const;

protected:
    void commitData(QWidget *editor);
};

#endif // DATATABLE_H
