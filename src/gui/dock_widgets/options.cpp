

#include "options.h"

#include <QDebug>

namespace MXit
{

namespace GUI
{

namespace DockWidget
{



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/
Options::Options(QWidget* parent, Theme &theme, QSettings& settings) : MXitDockWidget(parent, theme), settings(settings)
{
  setupUi(this);
  
  /* ======= general tab ======= */
  
  connect(autoLoginCheckBox, SIGNAL(clicked ()), this, SLOT(saveSettings ( )));
  /*TODO hide offline contacts*/
  
  connect(hideOfflineCheckBox, SIGNAL( clicked () ), this, SIGNAL(requestRefresh ( )));
  
  /* ======= gateway tab ======= */
  connect(httpRadioButton, SIGNAL(toggled ( bool )), httpWidget, SLOT(setEnabled ( bool )));
  
  connect(applyButton, SIGNAL(released ()), this, SLOT(emitGatewaySignal()));
  
  /*gateway settings save*/
  connect(httpRadioButton, SIGNAL(clicked ()), this, SLOT(saveGatewaySettings ( )));
  
    /*TODO send port and proxy down*/
  connect(httpProxyLineEdit, SIGNAL(editingFinished ()), this, SLOT(saveGatewaySettings ( )));
  connect(portLineEdit, SIGNAL(editingFinished ()), this, SLOT(saveGatewaySettings ( )));
  
  /* ======= theme tab ======= */
  connect(themeOpenButton, SIGNAL( released () )   , this, SLOT(openThemeBrowser ()));
  
  connect(
            themeComboBox, SIGNAL( currentIndexChanged ( const QString & ) ), 
            this, SLOT(loadTheme(const QString &)));
  connect(
            reloadButton, SIGNAL( released () ), 
            this, SLOT(reloadCurrentTheme()));
  
  /* ======= conversations tab ======= */
  connect(conversationsOpenButton, SIGNAL( released () )   , this, SLOT(openConversationsBrowser ()));
  
  connect(directoryLineEdit , SIGNAL(editingFinished ()), this, SLOT (refreshComboBox ())); 
  
  /*load settings*/
  autoLoginCheckBox->setChecked(settings.value("autoLogin").toBool());
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Options::~Options() {
  saveSettings();
  settings.sync();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Options::saveSettings() {

  settings.setValue("autoLogin", isAutoLogin());
}

/****************************************************************************
  _____                      __  ____        __  _         
 / ___/__ ___  ___ _______ _/ / / __/__ ____/ /_(_)__  ___ 
/ (_ / -_) _ \/ -_) __/ _ `/ / _\ \/ -_) __/ __/ / _ \/ _ \
\___/\__/_//_/\__/_/  \_,_/_/ /___/\__/\__/\__/_/\___/_//_/
                                                            
                               
****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

bool Options::isAutoLogin() {
  return autoLoginCheckBox->isChecked();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

bool Options::hideOfflineContacts() {
  return hideOfflineCheckBox->isChecked();
}

/****************************************************************************
  _____     __                         ____        __  _         
 / ___/__ _/ /____ _    _____ ___ __  / __/__ ____/ /_(_)__  ___ 
/ (_ / _ `/ __/ -_) |/|/ / _ `/ // / _\ \/ -_) __/ __/ / _ \/ _ \
\___/\_,_/\__/\__/|__,__/\_,_/\_, / /___/\__/\__/\__/_/\___/_//_/
                             /___/                               

****************************************************************************/


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::setSelectedGateway(const QString& gateway) {

  gatewayComboBox->setCurrentIndex( gatewayComboBox->findText ( gateway ));
}
  
/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::addGateway(const QString& gateway) {
  
  gatewayComboBox->addItem(gateway);
}

/****************************************************************************
**
** Author: Richard Baxter
** Author: Marc Bowes (proxy)
**
****************************************************************************/
void Options::emitGatewaySignal () {
  emit gatewaySelected(gatewayComboBox->currentText (), httpProxyLineEdit->text(), portLineEdit->text());
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::saveGatewaySettings() {
  /*TODO*/
  //settings->setValue("httpProxy", );
}

/****************************************************************************
 ________                   ____        __  _         
/_  __/ /  ___ __ _  ___   / __/__ ____/ /_(_)__  ___ 
 / / / _ \/ -_)  ' \/ -_) _\ \/ -_) __/ __/ / _ \/ _ \
/_/ /_//_/\__/_/_/_/\__/ /___/\__/\__/\__/_/\___/_//_/
                                                      
****************************************************************************/

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::openThemeBrowser () {
  QFileDialog fileDiag ( this, "Choose your theme directory");
  fileDiag.setFileMode(QFileDialog::DirectoryOnly);
  if (fileDiag.exec ()){
    QDir selection = fileDiag.selectedFiles ().front();
    
    setBaseThemeDirectory(selection.absolutePath ());
  }
}


/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Options::openConversationsBrowser () {
  QFileDialog fileDiag ( this, "Choose your Conversations directory");
  fileDiag.setFileMode(QFileDialog::DirectoryOnly);
  if (fileDiag.exec ()){
    QDir selection = fileDiag.selectedFiles ().front();
    
    setBaseConversationsDirectory(selection.absolutePath ());
  }
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


QString Options::getSelectedTheme() {
  return themeComboBox->currentText ();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


void Options::setSelectedTheme(const QString& theme) {
  qint32 selection = themeComboBox->findText (theme);
  if (selection == -1)
    selection = 0;
  themeComboBox->setCurrentIndex (selection);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/


QString Options::getBaseThemeDirectory() {
  return directoryLineEdit->text();
}

/****************************************************************************
**
** Author: Marc Bowes
**
****************************************************************************/
void Options::setBaseConversationsDirectory(const QString& dir) {
  /*TODO check if directory exists, if not, display it in red and return - rax*/
  conversationsLineEdit->setText(dir);
  QDir log(conversationsLineEdit->text());
  if (log.exists())
    emit conversationLogDirectorySelected(log);
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::setBaseThemeDirectory(const QString& dir) {
    /*TODO check if directory exists, if not, display it in red and return - rax*/
    directoryLineEdit->setText(dir);
  
    refreshComboBox();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Options::refreshComboBox ()
{
  QDir dir(directoryLineEdit->text());

  if (dir.exists()) {
    //directoryLineEdit->setForground(QBrush(Qt::black));
    themeComboBox->clear();
    QFileInfoList fil = dir.entryInfoList ( QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    
    Q_FOREACH(const QFileInfo& fileInfo, fil) {
      themeComboBox->addItem (fileInfo.fileName());
    }
  }
  else {
    //directoryLineEdit->setForground(QBrush(Qt::red));
    themeComboBox->clear();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Options::loadTheme(const QString &dir){
  QDir themeDir = getBaseThemeDirectory();
  themeDir.cd(dir);
  theme.load(themeDir);
  
  emit themeChanged();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::reloadCurrentTheme(){
  loadTheme(themeComboBox->currentText ());
}


} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */




