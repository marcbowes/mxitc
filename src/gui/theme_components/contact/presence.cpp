/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QImage>

#include "presence.h"

#define DEFAULT_SIZE QSize(16, 16)

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
** creates components from disk
**
****************************************************************************/
void Presence::load(const QDir &dir)
{
  /* load defaults */
  loadDefaults();
  
  /* load from disk */
  QImage _available   = QImage(dir.absoluteFilePath("available.png"));
  QImage _away        = QImage(dir.absoluteFilePath("away.png"));
  QImage _dnd         = QImage(dir.absoluteFilePath("dnd.png"));
  QImage _offline     = QImage(dir.absoluteFilePath("offline.png"));
  QImage _online      = QImage(dir.absoluteFilePath("online.png"));
  
  /* build pixmaps from images, or use defaults */
  if (!_available.isNull()) {
    available = QPixmap::fromImage(_available);
  }
  
  if (!_away.isNull()) {
    away = QPixmap::fromImage(_away);
  }
  
  if (!_dnd.isNull()) {
    dnd = QPixmap::fromImage(_dnd);
  }
  
  if (!_offline.isNull()) {
    offline = QPixmap::fromImage(_offline);
  }
  
  if (!_online.isNull()) {
    online = QPixmap::fromImage(_online);
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** creates components from disk
**
****************************************************************************/
void Presence::loadDefaults()
{
  /* build pixmaps from defaults */
  available = QPixmap(DEFAULT_SIZE);
  available.fill(Qt::blue);
  
  away = QPixmap(DEFAULT_SIZE);
  away.fill(QColor(255,215,0));

  dnd = QPixmap(DEFAULT_SIZE);
  dnd.fill(Qt::red);

  offline = QPixmap(DEFAULT_SIZE);
  offline.fill(Qt::gray);

  online = QPixmap(DEFAULT_SIZE);
  online.fill(Qt::green);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** returns the pixmap associated with the presence
**
****************************************************************************/
QPixmap Presence::pixmap(Protocol::Enumerables::Contact::Presence presence)
{
  using namespace Protocol::Enumerables::Contact;
  switch (presence) {
    case Available:
      return available;
      break;
    case Away:
      return away;
      break;
    case DoNotDisturb:
      return dnd;
      break;
    case Offline:
      return offline;
      break;
    case Online:
      return online;
      break;
    default:
      return QPixmap(DEFAULT_SIZE);
      break;
  }
}

}

}

}

}

