#ifndef KNOWNCHECKBOX_H
#define KNOWNCHECKBOX_H

#include <QCheckBox>

class KnownCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit KnownCheckBox(QWidget *parent = 0);

signals:
    void checked(KnownCheckBox *, bool);
public slots:
    void on_toggled(bool aChecked);
};

#endif // KNOWNCHECKBOX_H
