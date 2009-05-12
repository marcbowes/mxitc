/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QImage>

#include "theme.h"

#define DEFAULT_SIZE QSize(16, 16)

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
  /* load defaults */
  loadDefaults();
  
  /* base */
  /* load from disk */
  QImage _windowIcon  = QImage(theme.absoluteFilePath("icon.png"));
  
  /* build pixmaps from images, or use defaults */
  if (!_windowIcon.isNull()) {
    windowIcon = QPixmap::fromImage(_windowIcon);
  }
  
  /* components */
  if (theme.cd("contact")) {
    contact.load(theme);
    theme.cdUp();
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** constructs default theme components
**
****************************************************************************/
void Theme::loadDefaults()
{
  /* base */
  /* build pixmaps from defaults */
  windowIcon = QPixmap(DEFAULT_SIZE);
  windowIcon.fill(Qt::blue);
  
  /* components */
  contact.loadDefaults();
}

}

}

