/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Marc Bowes, 2009
** Author: Richard Baxter, 2009
**
** enumerables for presence settings
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_ENUMERABLES_PRESENCE_H__
#define __MXIT_PROTOCOL_ENUMERABLES_PRESENCE_H__

namespace MXit
{

namespace Protocol
{

namespace Enumerables
{

enum Presence {
  Offline         = 0,
  Online          = 1,
  Away            = 2,
  Available       = 3,
  DoNotDisturb    = 4
};

}

}

}

#endif /* __MXIT_PROTOCOL_ENUMERABLES_PRESENCE_H__ */

