/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
** 
** Author: Marc Bowes, 2009
**
** A list of possible errors reportable by MXit
**
****************************************************************************/

#ifndef __MXIT_PROTOCOL_ERROR_CODES_H__
#define __MXIT_PROTOCOL_ERROR_CODES_H__

#include <QString>

namespace MXit
{

namespace Protocol
{

namespace ErrorCodes
{
  static QString NoError                  = "0";  /* No error */
  static QString InvalidRegistration      = "3";  /* Invalid regisration */
  static QString AccountExpired           = "4";  /* Account expired */
  static QString CannotFindContact        = "7";  /* Cannot find contact to delete it */
  static QString MessageDeliveryFailed    = "15"; /* Message delivery failed */
  static QString RedirectToDifferentHost  = "16"; /* Redirect to different host */
  static QString InvalidSessionID         = "40"; /* Session ID invalid */
  static QString NotLoggedIn              = "42"; /* Not logged in */
  static QString UndefinedError           = "99"; /* Undefined error */
}

}

}

#endif

