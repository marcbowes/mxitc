/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** holder class for theme components
**
****************************************************************************/

#ifndef __MXIT_GUID_THEMECOMPONENTS_CONTACT_BASE_H__
#define __MXIT_GUID_THEMECOMPONENTS_CONTACT_BASE_H__

#include <QDir>

#include "presence.h"

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

namespace Contact
{

class Base
{
  public:         /* class specific */

  Base()  {}
  ~Base() {}

  public:         /* methods */
  
  void load(QDir theme);
  void loadDefaults();

  public:         /* variables */
  
  Presence presence;
};

}

}

}

}

#endif /* __MXIT_GUID_THEMECOMPONENTS_CONTACT_BASE_H__ */

