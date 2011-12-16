#ifndef UTILS_H
#define UTILS_H

#include "src/components/pagecomponent.h"

#include <math.h>

void copyVariable(PageComponent *aComponent);
void checkVarName(QString aName, QStringList &aErrorList);
QVariant calculatePart(QString aExpression, PageComponent *aComponent);

#endif // UTILS_H
