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
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(showQuitDialog()));
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
** Opens the login dialog
**
****************************************************************************/

void MXitC::openLoginWindow(){
  
  MXit::GUI::Login login(parentApp, mxit);
  login.exec();
  
}

/****************************************************************************
**
** Author: Richard Baxter
**
** Opens the 'are you sure you want to quit' dialog
**
****************************************************************************/

void MXitC::showQuitDialog ()
{
    QDialogButtonBox quitDialog (QDialogButtonBox::Yes|QDialogButtonBox::No, Qt::Horizontal, this);
    if (QMessageBox::question ( this ,/*QWidget * parent*/
                                      "Are you sure you want to quit?", /* const QString & title */
                                      "Are you sure you want to quit?", /* const QString & text */
                                      QDialogButtonBox::Ok|QDialogButtonBox::Cancel /*StandardButtons buttons = Ok*/ 
                                      /*StandardButton defaultButton = NoButton */))
    {
      // TODO log out if still logged in
      // TODO quit program...somehow
    }
    //else do nothing
    
  
}
  
  
}

}

