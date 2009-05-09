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

enum Contact {
  MXit        = 0,
  Jabber      = 1,
  Msn         = 2,
  Yahoo       = 3,
  Icq         = 4,
  Aim         = 5,
  Bot         = 8,
  ChatRoom    = 9,
  Gallary     = 12,
  Info        = 13,
  Multimx     = 14,
  GoogleTalk  = 18
};

}

}

}

#endif /* __MXIT_PROTOCOL_ENUMERABLES_CONTACT_H__ */

