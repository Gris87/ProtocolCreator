#ifndef UTILS_H
#define UTILS_H

#include "src/components/pagecomponent.h"
#include "src/components/variableextendedlistframe.h"

#include <math.h>

#include <QTextEdit>
#include <QTextTable>

void moveFile(const QString aSourceFileName, const QString aDestFileName);
void copyFile(const QString aSourceFileName, const QString aDestFileName);
void setGeometryInDesktop(QWidget* aWidget, int aX, int aY, int aWidthSize, int aHeightSize);
void findFocus(int &pageIndex, int &varIndex, int &compIndex);
void copyVariable(PageComponent *aComponent);
void checkVarName(QString aName, QStringList &aErrorList);
PageComponent* variableByName(QString aVariableName, PageComponent *aComponent);
PageComponent* getVariableOrThrow(QString aVariableName, PageComponent *aComponent);
QVariant calculatePart(QString aExpression, PageComponent *aComponent, VariableExtendedListFrame *inFrame=0, int tableRow=-1);
void replaceLinksInText(QTextEdit *aTextEdit);
QString variantToText(const QVariant &aVariant);

#endif // UTILS_H
