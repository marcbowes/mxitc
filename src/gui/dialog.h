/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "mxit/client.h"

#include "ui_dialog.h"

namespace MXit
{

namespace GUI
{

class Dialog : public QMainWindow, private Ui::Dialog
{
  Q_OBJECT

  public:
  
  Dialog(QApplication *app);
  ~Dialog();
  
  private:
  
  Client *mxit;
  
  private slots:
  
  void captchaRespond();
  void captchaResponseChanged(const QString &text);
};

}

}
 
#endif

