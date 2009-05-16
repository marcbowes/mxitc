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
    htmlStylesheet = QLatin1String(css.readAll());
  }
  
  /* stylesheet */
  QFile qss(theme.filePath("stylesheet.qss"));
  qss.open(QFile::ReadOnly);
  if (qss.exists()) {
    stylesheet = QLatin1String(qss.readAll());
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
  htmlStylesheet = QString();
  stylesheet     = QString();
}

}

}

}

