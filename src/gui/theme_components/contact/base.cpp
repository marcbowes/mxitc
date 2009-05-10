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
  /* stylesheet */
  QFile qss ("stylesheet.qss");
  qss.open(QFile::ReadOnly);
  if (qss.exists()) {
    stylesheet = QLatin1String(qss.readAll());
  } else {
    /* ensure it's empty, in case of loading a new theme with no qss */
    stylesheet = QString();
  }
  
  /* load theme components */
  if (theme.cd("presence")) {
    presence.load(theme);
    theme.cdUp();
  } else {
    presence.loadDefaults();
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** loads defaults
**
****************************************************************************/
void Base::loadDefaults()
{
  presence.loadDefaults();
}

}

}

}

}

