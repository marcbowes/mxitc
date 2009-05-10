/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "theme.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Marc Bowes
**
** constructs theme components
**
****************************************************************************/
void Theme::load(QDir theme)
{  
  if (theme.cd("contact")) {
    contact.load(theme);
    theme.cdUp();
  } else {
    contact.loadDefaults();
  }
}

}

}

