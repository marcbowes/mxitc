/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** This is an extension of the generated dialog, made with Designer
**
****************************************************************************/

#include "login.h"

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
Login::Login(QApplication *app, MXit::Client *client)
{
  setupUi(this);      /* from ui_dialog.h: generated from dialog.ui */
  mxit = client;      /* store a copy */
  
  /* add in an image to display the login CAPTCHA */
  mxit->getLoginCaptcha();
    
  /* enable/disable 'Login' based on the validity of the user inputs */
  connect(cellphone, SIGNAL(textChanged(const QString &)), 
          this, SLOT(cellphoneChanged(const QString &)));
  connect(password, SIGNAL(textChanged(const QString &)), 
          this, SLOT(passwordResponseChanged(const QString &)));
  connect(captcha, SIGNAL(textChanged(const QString &)), 
          this, SLOT(captchaChanged(const QString &)));
  
  /* when 'Login' is clicked, or the user presses return */
  connect(cellphone, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(password, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(captcha, SIGNAL(returnPressed()), this, SLOT(login()));
  connect(loginButton, SIGNAL(released()), this, SLOT(login()));
  
  /* when a CAPTCHA is received from the MXit gateway, display it */
  connect(mxit, SIGNAL(captchaReceived(const QByteArray &)), this, SLOT(captchaReceived(const QByteArray &)));
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** Dialog deconstructor
**
****************************************************************************/
Login::~Login()
{
  // nothing here
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered when cellphone input text is changed
**
****************************************************************************/
void Login::captchaChanged(const QString &text)
{
  loginButton->setDisabled(text.isEmpty() ? true : false);
}


/****************************************************************************
**
** Copyright 2009 Timothy Sjoberg
**
** this SLOT is triggered when the client receives the CAPTCHA
**
****************************************************************************/
void Login::captchaReceived(const QByteArray &captcha)
{
  QImage captchaImage;
  captchaImage.loadFromData(captcha);
  captchaLabel->setPixmap(QPixmap::fromImage(captchaImage));
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered when cellphone input text is changed
**
****************************************************************************/
void Login::cellphoneChanged(const QString &text)
{
  loginButton->setDisabled(text.isEmpty() ? true : false);
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered by pressing 'Login' or typing return
**
****************************************************************************/
void Login::login()
{
  if (!captcha->text().isEmpty()) {
    loginButton->setDisabled(true);
    loginButton->setText("Logging in..");
    mxit->sendChallengeResponse(captcha->text(), cellphone->text());
  }
}


/****************************************************************************
**
** Copyright 2009 Marc Bowes
**
** this SLOT is triggered when password input text is changed
**
****************************************************************************/
void Login::passwordChanged(const QString &text)
{
  loginButton->setDisabled(text.isEmpty() ? true : false);
}

}

}
