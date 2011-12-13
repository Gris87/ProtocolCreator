#ifndef WORDTEXTEDIT_H
#define WORDTEXTEDIT_H

#include <QTextEdit>

#include <QKeyEvent>

class WordTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit WordTextEdit(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *e);

signals:
    void boldCombination();
    void italicCombination();
    void underlineCombination();
};

#endif // WORDTEXTEDIT_H
