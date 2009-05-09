/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
** Abstract class that stores all data relavant to a single contact
**
****************************************************************************/

#ifndef __MXIT_GUI_CONTACT_H__
#define __MXIT_GUI_CONTACT_H__

#include <QObject>
#include <QIcon>
#include "common/types.h"
#include "message.h"
#include "contact_metadata.h"

namespace MXit
{

namespace GUI
{


class Contact : public QObject
{
  Q_OBJECT
  
  public:         /* class specific */
	
  Contact(
      const QString& group = "",
      const QString& contactAddress = "",
      const QString& nickname = "",
      unsigned int presence = 0,
      unsigned int type = 0, 
      unsigned int mood = 0);
      
      
  Contact(const Contact& c);
      
  ~Contact();
  
  Contact& operator=(const Contact&);


  ContactMetaData * metaData;

  
  public:         /* methods */

  /*TODO add getters and setter if needed*/
  QString getGroup() const;
  QString getContactAddress() const;
  QString getNickname() const;
  
  void setGroup(const QString& group);
  void setContactAddress(const QString& contactAddress);
  void setNickname(const QString& nickname);
  void setPresence(int presence);
  void setType(int type);
  void setMood(int mood);
  
  void setChatInputText(const QString& inputChatText);
  QString getChatInputText();


  QVector <Message> chatHistory;

  bool unreadMessage;
  Presence presence;
  Type type;
  Mood mood;

  /* group0 \1 contactAddress0 \1 nickname0 \1 presence0 \1 type0 \1 mood \0 */
  private: /* variables */
  QString group;
  QString contactAddress;
  QString nickname;
  
  QString chatInputText; /* an area to save what the user has types in the input line */
  


};



} /* end namespace GUI */

} /* end namespace MXit */

#endif




