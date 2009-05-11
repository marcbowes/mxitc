/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QImage>

#include "type.h"

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
void Type::load(const QDir &dir)
{
  /* load defaults */
  loadDefaults();
  
  /* load from disk */
  QImage _aim         = QImage(dir.absoluteFilePath("aim.png"));
  QImage _bot         = QImage(dir.absoluteFilePath("bot.png"));
  QImage _chat_room   = QImage(dir.absoluteFilePath("chat_room.png"));
  QImage _gallery     = QImage(dir.absoluteFilePath("gallery.png"));
  QImage _google_talk = QImage(dir.absoluteFilePath("google_talk.png"));
  QImage _icq         = QImage(dir.absoluteFilePath("icq.png"));
  QImage _info        = QImage(dir.absoluteFilePath("info.png"));
  QImage _jabber      = QImage(dir.absoluteFilePath("jabber.png"));
  QImage _msn         = QImage(dir.absoluteFilePath("msn.png"));
  QImage _multimx     = QImage(dir.absoluteFilePath("multimx.png"));
  QImage _mxit        = QImage(dir.absoluteFilePath("mxit.png"));
  QImage _yahoo       = QImage(dir.absoluteFilePath("yahoo.png"));
  
  /* build pixmaps from images, or use defaults */
  if (!_aim.isNull()) {
    aim = QPixmap::fromImage(_aim);
  }
  
  if (!_bot.isNull()) {
    bot = QPixmap::fromImage(_bot);
  }
  
  if (!_chat_room.isNull()) {
    chat_room = QPixmap::fromImage(_chat_room);
  }
  
  if (!_gallery.isNull()) {
    gallery = QPixmap::fromImage(_gallery);
  }
  
  if (!_google_talk.isNull()) {
    google_talk = QPixmap::fromImage(_google_talk);
  }
  
  if (!_icq.isNull()) {
    icq = QPixmap::fromImage(_icq);
  }
  
  if (!_info.isNull()) {
    info = QPixmap::fromImage(_info);
  }
  
  if (!_jabber.isNull()) {
    jabber = QPixmap::fromImage(_jabber);
  }
  
  if (!_msn.isNull()) {
    msn = QPixmap::fromImage(_msn);
  }
  
  if (!_multimx.isNull()) {
    multimx = QPixmap::fromImage(_multimx);
  }
  
  if (!_mxit.isNull()) {
    mxit = QPixmap::fromImage(_mxit);
  }
  
  if (!_yahoo.isNull()) {
    yahoo = QPixmap::fromImage(_yahoo);
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
** creates components from disk
**
****************************************************************************/
void Type::loadDefaults()
{
  /* build pixmaps from defaults */
  aim = QPixmap(DEFAULT_SIZE);
  aim.fill(Qt::gray);
  
  bot = QPixmap(DEFAULT_SIZE);
  bot.fill(Qt::gray);
  
  chat_room = QPixmap(DEFAULT_SIZE);
  chat_room.fill(Qt::gray);
  
  gallery = QPixmap(DEFAULT_SIZE);
  gallery.fill(Qt::gray);
  
  google_talk = QPixmap(DEFAULT_SIZE);
  google_talk.fill(Qt::gray);
  
  icq = QPixmap(DEFAULT_SIZE);
  icq.fill(Qt::gray);
  
  info = QPixmap(DEFAULT_SIZE);
  info.fill(Qt::gray);
  
  jabber = QPixmap(DEFAULT_SIZE);
  jabber.fill(Qt::gray);
  
  msn = QPixmap(DEFAULT_SIZE);
  msn.fill(Qt::gray);
  
  mxit = QPixmap(DEFAULT_SIZE);
  mxit.fill(Qt::gray);
  
  multimx = QPixmap(DEFAULT_SIZE);
  multimx.fill(Qt::gray);
  
  yahoo = QPixmap(DEFAULT_SIZE);
  yahoo.fill(Qt::gray);
}


/****************************************************************************
**
** Author: Marc Bowes
**
** returns the pixmap associated with the type
**
****************************************************************************/
QPixmap Type::pixmap(Protocol::Enumerables::Contact::Type type)
{
  using namespace Protocol::Enumerables::Contact;
  switch (type) {
    case AIM:
      return aim;
      break;
    case Bot:
      return bot;
      break;
    case ChatRoom:
      return chat_room;
      break;
    case Gallery:
      return gallery;
      break;
    case GoogleTalk:
      return google_talk;
      break;
    case ICQ:
      return icq;
      break;
    case Info:
      return info;
      break;
    case Jabber:
      return jabber;
      break;
    case MSN:
      return msn;
      break;
    case MultiMx:
      return multimx;
      break;
    case MXit:
      return mxit;
      break;
    case Yahoo:
      return yahoo;
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

