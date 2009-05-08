/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
** Class that stores all info relevant to one message send
**
****************************************************************************/

#ifndef __MXIT_GUI_MESSAGE_H__
#define __MXIT_GUI_MESSAGE_H__

#include <QObject>

namespace MXit
{

namespace GUI
{

class Contact;

class Message : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */
	
  Message(const Contact * sender = 0, const QString & message = "");
  Message(const Message & m);
  
  ~Message();
  
  const Contact * getSender() const;
  QString getMessage() const;
  QString getFormattedMsg() const;
  
  Message& operator=(const Message & );
  
  private:
  
  QString message;
  const Contact  * sender;

};


} /* end namespace GUI */

} /* end namespace MXit */

#endif





