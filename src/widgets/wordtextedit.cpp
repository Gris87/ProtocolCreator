#include "wordtextedit.h"

WordTextEdit::WordTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}

void WordTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (!isReadOnly())
    {
        if (e==QKeySequence::Bold)
        {
            emit boldCombination();
            return;
        }
        if (e==QKeySequence::Italic)
        {
            emit italicCombination();
            return;
        }
        if (e==QKeySequence::Underline)
        {
            emit underlineCombination();
            return;
        }
    }

    QTextEdit::keyPressEvent(e);
}
