#include "knowncheckbox.h"

KnownCheckBox::KnownCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    connect(this, SIGNAL(toggled(bool)), this, SLOT(on_toggled(bool)));
}

void KnownCheckBox::on_toggled(bool aChecked)
{
    emit checked(this, aChecked);
}
