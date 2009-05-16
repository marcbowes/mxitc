/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "chat.h"

namespace MXit
{

namespace GUI
{

namespace ThemeComponents
{

/****************************************************************************
**
** Author: Marc Bowes
**
** loads components
**
****************************************************************************/
void Chat::load(QDir theme)
{
  /* html stylesheet */
  QFile css(theme.filePath("stylesheet.css"));
  css.open(QFile::ReadOnly);
  if (css.exists()) {
    stylesheet = QLatin1String(css.readAll());
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** loads defaults
**
****************************************************************************/
void Chat::loadDefaults()
{
  stylesheet     = QString();
}

}

}

}

