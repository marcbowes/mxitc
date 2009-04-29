/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::MxitDialog is a generalisd dialog for the PC MXit client
**
****************************************************************************/

#ifndef __MXIT_GUI_MXIT_DIALOG_H__
#define __MXIT_GUI_MXIT_DIALOG_H__

#include <QDialog>
#include <QSettings>

#include "mxit/client.h"


namespace MXit
{

namespace GUI
{

class MXitDialog : public QDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitDialog(QWidget *parent = 0, MXit::Client *client = 0, QSettings *settings = 0);
  ~MXitDialog();

  private: /* methods */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public slots:

  protected slots:
  
  private slots:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  protected:        /* variables */
  
  MXit::Client *mxit;
  
  QSettings *settings;
  
};

}

}
 
#endif

