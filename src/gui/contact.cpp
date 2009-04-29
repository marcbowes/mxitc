/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QDebug>

#include "contact.h"

namespace MXit
{

/****************************************************************************
**
** Author: Richard Baxter
**
** Contact constructor
**
****************************************************************************/

Contact::Contact(const QString& name, const QString& cellphone) : name (name), cellphone(cellphone)
{
  /* nothing */
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Contact deconstructor
**
****************************************************************************/
Contact::~Contact()
{
  /* nothing */
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getName()
{
  return name;
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Contact::getCellphone()
{
  return cellphone;
}


} /* end namespace MXit */


