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
  
  /* enable/disable 'Respond' based on the length of the user CAPTCHA response */
  connect(captchaResponse, SIGNAL(textChanged(QString)), this, SLOT(captchaResponseChanged(QString)));
  
  /* when 'Respond' is clicked, or the user presses return, send the CAPTCHA response */
  connect(captchaRespondButton, SIGNAL(released()), this, SLOT(captchaRespond()));
  connect(captchaResponse, SIGNAL(returnPressed()), this, SLOT(captchaRespond()));
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


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered when the 'Respond' is clicked or the user presses
** return while in the CAPTCHA input
**
****************************************************************************/
void Dialog::captchaRespond()
{
  if (!captchaResponse->text().isEmpty())
    mxit->sendCaptchaResponse(captchaResponse->text());
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered when the text is changed in the CAPTCHA input box
** depending on the length, the 'Repond' button is enabled or disabled
**
****************************************************************************/
void Dialog::captchaResponseChanged(const QString &text)
{
  captchaRespondButton->setDisabled(text.isEmpty() ? true : false);
}

