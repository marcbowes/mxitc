/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_MXITC_H__
#define __MXIT_GUI_MXITC_H__

#include <QMessageBox>
#include <QDialogButtonBox>
#include <QCloseEvent>

#include "mxit/client.h"
#include "gui/login.h"

#include "ui_mxitc.h"

namespace MXit
{

namespace GUI
{

class MXitC : public QMainWindow, private Ui::MXitC
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitC(QApplication *app, MXit::Client *client = 0);
  ~MXitC();


  public slots:
  
  void incomingMessage(const QString & message);
  void outgoingMessage(const QString & message);

  protected slots:
  
  virtual void closeEvent(QCloseEvent *event);

  private slots:
  
  void openLoginWindow();
  
  private:        /* variables */
  
  MXit::Client *mxit;
  QApplication *application;
};

}

}
 
#endif

