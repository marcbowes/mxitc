/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "base.h"

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

namespace Contact
{

/****************************************************************************
**
** Author: Marc Bowes
**
** loads components
**
****************************************************************************/
void Base::load(QDir theme)
{
  /* load theme components */
  if (theme.cd("presence")) {
    presence.load(theme);
    theme.cdUp();
  }
}

}

}

}

}

