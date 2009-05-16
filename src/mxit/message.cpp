/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QRegExp>
#include <QTextDocument>

#include "message.h"

namespace MXit
{

/****************************************************************************
        __                                _ ____    
   ____/ /__ ____ ___   ___ ___  ___ ____(_) _(_)___
  / __/ / _ `(_-<(_-<  (_-</ _ \/ -_) __/ / _/ / __/
  \__/_/\_,_/___/___/ /___/ .__/\__/\__/_/_//_/\__/ 
                         /_/                        

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for outgoing chat (to a Contact)
**
****************************************************************************/
Message::Message(const QString &message)
  : contact(NULL), message(markup(message)), timestamp(QTime::currentTime())
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Constructor for incoming chat (from a Contact)
**
****************************************************************************/
Message::Message(const Contact &contact, const QString &message)
  : contact(&contact), message(markup(message)), timestamp (QTime::currentTime())
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Copy constructor
**
****************************************************************************/
Message::Message(const Message &other)
  : contact(other.contact), message(other.message), timestamp(other.timestamp)
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Default destructor
**
****************************************************************************/
Message::~Message()
{
  /* Nothing */
}


/****************************************************************************
**
** Author: Marc Bowes
**
** Implements MXit Markup
** All text is first CGI escaped, and then markup-up into HTML using MXit-
**  specifc rules.
**
****************************************************************************/
QString Message::markup(const QString &markup)
{
  /* CGI escape so that any HTML in the message isn't interpreted */
  QString markedUp = Qt::escape(markup);
  
  /* roll through each rule */
  QRegExp rx;
  rx.setMinimal(true);
  
  /* /word/ = <i>word</i> */
  rx.setPattern("\\/(.+)\\/");
  markedUp.replace(rx, "<i>\\1</i>");
  
  /* *word* = <b>word</b> */
  rx.setPattern("\\*(.+)\\*");
  markedUp.replace(rx, "<b>\\1</b>");
  
  /* _word_ = <u>word</u> */
  rx.setPattern("_(.+)_");
  markedUp.replace(rx, "<u>\\1</u>");
  
  return markedUp;
}

}

