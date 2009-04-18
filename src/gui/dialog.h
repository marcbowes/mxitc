/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "mxit/client.h"
#include "network/httpcomm.h"

#include "ui_dialog.h"

namespace MXit
{

namespace GUI
{

class Dialog : public QMainWindow, private Ui::Dialog
{
  Q_OBJECT

	public:         /* class specific */
  
  Dialog(QApplication *app);
  ~Dialog();

  private slots:
  
  void captchaReceived(const QByteArray &captcha);
  void captchaRespond();
  void captchaResponseChanged(const QString &text);
  
	private:        /* variables */
  
  Client *mxit;
  HttpComm * httpComm;
};

}

}
 
#endif

