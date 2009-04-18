#include "dialog.h"

/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** Dialog constructor
**
** ==== Arguments
** - QApplication: defined in main.cpp from cmdline args
**
****************************************************************************/
Dialog::Dialog(QApplication *app)
{
  setupUi(this); /* from ui_dialog.h: generated from dialog.ui */
  mxit = new Mxit();
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
Dialog::~Dialog()
{
  delete mxit;
}

