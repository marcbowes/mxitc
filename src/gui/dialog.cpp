/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#include "dialog.h"

namespace MXit
{

namespace GUI

{

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
  mxit = new Client();  /* this is our slave client */
  
  /* add in an image to display the login CAPTCHA */
  mxit->getLoginCaptcha();
    
  /* enable/disable 'Respond' based on the length of the user CAPTCHA response */
  connect(captchaResponse, SIGNAL(textChanged(const QString &)), 
          this, SLOT(captchaResponseChanged(const QString &)));
  
  /* when 'Respond' is clicked, or the user presses return, send the CAPTCHA response */
  connect(captchaRespondButton, SIGNAL(released()), this, SLOT(captchaRespond()));
  connect(captchaResponse, SIGNAL(returnPressed()), this, SLOT(captchaRespond()));
  
  connect(mxit, SIGNAL(captchaReceived(const QByteArray &)), this, SLOT(captchaReceived(const QByteArray &)));
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
** Copyright 2009 Timothy Sjoberg
**
** this SLOT is triggered when the client receives the CAPTCHA
**
****************************************************************************/
void Dialog::captchaReceived(const QByteArray &captcha)
{
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
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

}

}
