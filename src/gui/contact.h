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
#include "common/types.h"
#include "message.h"

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

  /* 
  Presence:
  The following presence states are defined:
          0 – offline
          1 – online
          2 – away (but still online)
          3 – available (deprecated, use online)
          4 – do not disturb
  Type:
  The following contact types are defined:
          0 – MXit
          1 – Jabber
          2 – MSN
          3 – Yahoo
          4 – ICQ
          5 – AIM
          8 – Bot
          9 – Chat room
          12 – Gallery
          13 – Info
          14 – MultiMx
          18 – Google Talk
  Mood:
  The following moods are defined:
          0 – None
          1 – Angry
          2 – Excited
          3 – Grumpy
          4 – Happy
          5 – In love
          6 – Invincible
          7 – Sad
          8 – Hot
          9 – Sick
          10 – Sleepy 
        */
        
  /* Presence */
  enum Presence {
  OFFLINE         = 0,
  ONLINE          = 1,
  AWAY            = 2,
  AVAILABLE       = 3,
  DO_NOT_DISTURB  = 4};
  
  /* Type */
  enum Type {
  MXIT        = 0,
  JABBER      = 1,
  MSN         = 2,
  YAHOO       = 3,
  ICQ         = 4,
  AIM         = 5,
  BOT         = 8,
  CHAT_ROOM   = 9,
  GALLARY     = 12,
  INFO        = 13,
  MULTIMX     = 14,
  GOOGLE_TALK = 18};
  
  /* Mood */
  enum Mood {
  NONE        = 0,
  ANGRY       = 1,
  EXCITED     = 2,
  GRUMPY      = 3,
  HAPPY       = 4,
  IN_LOVE     = 5,
  INVINCIBLE  = 6,
  SAD         = 7,
  HOT         = 8,
  SICK        = 9,
  SLEEPY      = 10};
  
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




