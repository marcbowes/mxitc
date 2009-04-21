/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "mxitc.h"

namespace MXit
{

namespace GUI

{

/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog constructor
**
** ==== Arguments
** - app: owned by main.cpp
** - client: owned by main.cpp
**
****************************************************************************/
MXitC::MXitC(QApplication *app, MXit::Client *client)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = client;      /* store a copy */
  parentApp = app;    /* store a copy */

  connect(actionLogon_to_MXIT, SIGNAL(triggered()), this, SLOT(openLoginWindow()));
}


/****************************************************************************
**
** Author: Richard Baxter
**
** Dialog deconstructor
**
****************************************************************************/
MXitC::~MXitC()
{
  // nothing here
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the login window (FIXME, should be dialog)
**
****************************************************************************/

void MXitC::openLoginWindow(){
  
  MXit::GUI::Login login(parentApp, mxit);
  login.show();
  
}
  
  
}

}

