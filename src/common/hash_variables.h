/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** maps variables to values
**
****************************************************************************/

#ifndef __MXIT_HASH_VARIABLES_H__
#define __MXIT_HASH_VARIABLES_H__

#include "types.h"

namespace MXit
{

VariableHash hashVariables(const QByteArray &data, const StringVec &variables,
    char delimiter = ';');

}

#endif

