/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "chat.h"

#define DEFAULT_SIZE QSize(16, 16)

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
  
  /* load from disk */
  QImage _group = QImage(theme.absoluteFilePath("group.png"));
  
  /* build pixmaps from images, or use defaults */
  if (!_group.isNull()) {
    group = QPixmap::fromImage(_group);
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
  stylesheet = QString();
  
  group = QPixmap(DEFAULT_SIZE);
  group.fill(Qt::black);
}

}

}

}

