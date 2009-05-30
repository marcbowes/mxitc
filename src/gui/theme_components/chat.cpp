/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "gui/emoticon.h"

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
  QImage _group  = QImage(theme.absoluteFilePath("group.png"));
  QImage _unread = QImage(theme.absoluteFilePath("unread.png"));
  
  /* build pixmaps from images, or use defaults */
  if (!_group.isNull()) {
    group = QPixmap::fromImage(_group);
  }
  
  if (!_unread.isNull()) {
    unread = QPixmap::fromImage(_unread);
  }
  
  /* load emoticons */
  emoticons.clear();
  if (theme.cd("emoticons")) {
    QStringList filter; filter << "*.png";
    QStringList files = theme.entryList(filter);
    Q_FOREACH(const QString &file, files) {
      /* file = semicolon.rightbracket.png */
      int idx = file.lastIndexOf('.');
      if (idx == -1) continue; /* safety check, even though filter.. */
      QString fileWithoutExtension = file.left(idx);
      /* fileWithoutExtension = semicolon.rightbracket */
      QString emoticon = Emoticon::spokenToShorthand(fileWithoutExtension);
      if (emoticon.isEmpty()) continue; /* dictionary can't translate */
      /* emoticon = :) */
      emoticons[emoticon] = QPixmap::fromImage(QImage(theme.absoluteFilePath(fileWithoutExtension)));
      /* emoticons[":)"] = 
                                .-""""""-.
                              .'          '.
                             /   O      O   \
                            :                :
                            |                |
                            : ',          ,' :
                             \  '-......-'  /
                              '.          .'
                                '-......-' */
    }
    theme.cdUp();
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
  emoticons.clear();
  
  group = QPixmap(DEFAULT_SIZE);
  group.fill(Qt::black);
  
  unread = QPixmap(DEFAULT_SIZE);
  unread.fill(Qt::yellow);  
}

}

}

}

