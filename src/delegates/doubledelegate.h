#ifndef DOUBLEDELEGATE_H
#define DOUBLEDELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QDoubleSpinBox>

class DoubleDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    quint8 mDecimals;
    QString mPrefix;
    QString mPostfix;

    DoubleDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DOUBLEDELEGATE_H
