/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QRegExp>

#include "emoticon.h"

namespace MXit
{

namespace GUI
{

/****************************************************************************
**
** Author: Marc Bowes
**
** Constructrs from params
**
****************************************************************************/
Emoticon::Emoticon(const QString &shorthand, const QString &spoken, const QString &file)
  : shorthand(shorthand), spoken(spoken), file(file)
{
  /* nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Replaces emoticon img tags with the shorthand text
**
****************************************************************************/
void Emoticon::HtmlToShorthand(QString &message)
{
  QRegExp rx("<img alt=\"(.*)\" src=\"(.*)\" class=\"emoticon\" />");
  rx.setMinimal(true);
  message.replace(rx, "\\1");
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Replaces shorthand with and HTML img tag
**
****************************************************************************/
void Emoticon::shorthandToHtml(QString &message, const QString &path) const
{
  message.replace(shorthand, QString("<img alt=\"%1\" src=\"%2%3\" class=\"emoticon\" />").arg(shorthand).arg(path).arg(file));
}

}

}

