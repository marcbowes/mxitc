/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QHash>

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
** spoken -> shorthand translation.
**
****************************************************************************/
QString Chat::spokenToShorthand(const QString &spoken)
{
  static QHash<QString, QChar> dictionary;
  static bool dictionaryLoaded (false); /* only set false first time */
  
  /* build dictionary if it wasn't previously built */
  if (!dictionaryLoaded) {
    /* ordered alphabetically by spoken */
    dictionary["leftbracket"]   = '(';
    dictionary["rightbracket"]  = ')';
    dictionary["colon"]         = ':';
    
    dictionaryLoaded = true;
  }
  
  /* do translation */
  QString shorthand;
  Q_FOREACH(const QString &string, spoken.split(".")) {
    if (!dictionary.contains(string))
      return QString(); /* no match */
    else
      shorthand.append(dictionary.value(string));
  }
  
  /* returns ":)" for "semicolon.rightbracket" */
  return shorthand;
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Takes a message and replaces text with emoticon images (HTML img)
**
****************************************************************************/
void Chat::injectEmoticons(QString &message) const
{
  Q_FOREACH(const Emoticon &emoticon, emoticons)
    emoticon.shorthandToHtml(message, theme.canonicalPath() + "/emoticons/");
}


/****************************************************************************
**
** Author: Marc Bowes
**
** loads components
**
****************************************************************************/
void Chat::load(QDir theme)
{
  this->theme = theme;
  
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
    QStringList files = theme.entryList();
    Q_FOREACH(const QString &file, files) {
      /* file = colon.rightbracket.gif */
      int idx = file.lastIndexOf('.');
      if (idx == -1) continue; /* safety check, even though filter.. */
      QString spoken = file.left(idx);
      /* fileWithoutExtension = colon.rightbracket */
      QString shorthand = spokenToShorthand(spoken);
      if (shorthand.isEmpty()) continue; /* dictionary can't translate */
      /* shorthand = :) */
      emoticons << Emoticon(shorthand, spoken, file);
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

