#include "hash_variables.h"

namespace MXit
{

/****************************************************************************
**
** Author: Marc Bowes
** Author: Richard Baxter
**
** maps variables to values
**
****************************************************************************/
VariableHash hashVariables(const QByteArray &data, const StringVec &variables,
  char delimiter)
{
  /* setup */
  VariableHash params;                    /* to be returned */
  StringVecItr itr(variables);            /* for iteration */
  unsigned int start  = 0;                /* stores index of a value start */
  int end             = data.indexOf(delimiter);
                                          /* stores index of a value end */
  /* iterate over variable list */
  while (itr.hasNext()) {
    const QString &key = itr.next();      /* current variable name */
    
    /* end will be -1 if the indexOf returns -1 */
    if (end != -1) {
      /* store string between delimiters as the value of the variable */
      params[key] = data.mid(start, end - start);
      
      /* next iteration setup */
      start = end + 1;
      end = data.indexOf(delimiter, start);
    } else {
      /* i.e. we want to have empty values for the remaining variables */
      params[key] = data.mid(start);
    }
  }
  
  return params;
}

}

