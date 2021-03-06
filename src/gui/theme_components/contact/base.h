/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
**
** holder class for theme components
**
****************************************************************************/

#ifndef __MXIT_GUI_THEMECOMPONENTS_CONTACT_BASE_H__
#define __MXIT_GUI_THEMECOMPONENTS_CONTACT_BASE_H__

#include <QDir>
#include <QString>

#include "presence.h"
#include "type.h"

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
  
  Presence  presence;
  QString   stylesheet;
  Type      type;
};

}

}

}

}

#endif /* __MXIT_GUI_THEMECOMPONENTS_CONTACT_BASE_H__ */

