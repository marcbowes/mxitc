/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** enumerables for contact types
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_ENUMERABLES_CONTACT_H__
#define __MXIT_PROTOCOL_ENUMERABLES_CONTACT_H__

namespace MXit
{

namespace Protocol
{

namespace Enumerables
{

namespace Contact
{

enum Mood {
  None           = 0,
  Angry          = 1,
  Excited        = 2,
  Grumpy         = 3,
  Happy          = 4,
  InLove         = 5,
  Invincible     = 6,
  Sad            = 7,
  Hot            = 8,
  Sick           = 9,
  Sleepy         = 10
};

enum Presence {
  Offline        = 0,
  Online         = 1,
  Away           = 2,
  Available      = 3,
  DoNotDisturb   = 4
};

enum Type {
  MXit           = 0,
  Jabber         = 1,
  Msn            = 2,
  Yahoo          = 3,
  Icq            = 4,
  Aim            = 5,
  Bot            = 8,
  ChatRoom       = 9,
  Gallary        = 12,
  Info           = 13,
  Multimx        = 14,
  GoogleTalk     = 18
};


enum AlertProfile {General, Meeting, Silent};

}

}

}

}

#endif /* __MXIT_PROTOCOL_ENUMERABLES_CONTACT_H__ */

