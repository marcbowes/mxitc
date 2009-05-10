/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QImage>

#include "presence.h"

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
** creates components from disk
**
****************************************************************************/
void Presence::load(const QDir &dir)
{
  /* load from disk */
  QImage _available   = QImage(dir.absoluteFilePath("presence/available.png"));
  QImage _away        = QImage(dir.absoluteFilePath("presence/away.png"));
  QImage _dnd         = QImage(dir.absoluteFilePath("presence/dnd.png"));
  QImage _offline     = QImage(dir.absoluteFilePath("presence/offline.png"));
  QImage _online      = QImage(dir.absoluteFilePath("presence/online.png"));
  
  /* build pixmaps from images, or use defaults */
  if (_available.isNull()) {
    available = QPixmap(DEFAULT_SIZE);
    available.fill(Qt::blue);
  } else {
    available = QPixmap::fromImage(_available);
  }
  
  if (_away.isNull()) {
    away = QPixmap(DEFAULT_SIZE);
    away.fill(QColor(255,215,0));
  } else {
    away = QPixmap::fromImage(_away);
  }
  
  if (_dnd.isNull()) {
    dnd = QPixmap(DEFAULT_SIZE);
    dnd.fill(Qt::red);
  } else {
    dnd = QPixmap::fromImage(_dnd);
  }
  
  if (_offline.isNull()) {
    offline = QPixmap(DEFAULT_SIZE);
    offline.fill(Qt::gray);
  } else {
    offline = QPixmap::fromImage(_offline);
  }
  
  if (_online.isNull()) {
    online = QPixmap(DEFAULT_SIZE);
    online.fill(Qt::green);
  } else {
    online = QPixmap::fromImage(_online);
  }
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
  switch (presence) {
    case Protocol::Enumerables::Contact::Available:
      return available;
      break;
    case Protocol::Enumerables::Contact::Away:
      return away;
      break;
    case Protocol::Enumerables::Contact::DoNotDisturb:
      return dnd;
      break;
    case Protocol::Enumerables::Contact::Offline:
      return offline;
      break;
    case Protocol::Enumerables::Contact::Online:
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

