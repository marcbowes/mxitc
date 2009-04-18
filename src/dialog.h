/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "mxit.h"

#include "ui_dialog.h"

class Dialog : public QMainWindow, private Ui::Dialog
{
  Q_OBJECT

  public:
  
  Dialog(QApplication *app);
  ~Dialog();
  
  private:
  
  Mxit *mxit;
};
 
#endif

