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
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = new Mxit();  /* this is our slave client */
  
  /* add in an image to display the login CAPTCHA */
  QImage captcha;
  captcha.loadFromData(mxit->getLoginCaptcha());
  captchaLabel->setPixmap(QPixmap::fromImage(captcha));
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

