

#include "options.h"

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
Options::Options(QWidget* parent, Theme &theme, MXit::Client& mxit, QSettings& settings) : MXitDockWidget(parent, theme), mxit(mxit), settings(settings), loadingSettings(false)
{
  setupUi(this);
  
 
  loadingSettings = true;
  
  /* ======= general tab ======= */
  
  connect(autoLoginCheckBox, SIGNAL(clicked ()), this, SLOT(saveSettings ( )));
  /*TODO hide offline contacts*/
  
  connect(hideOfflineCheckBox, SIGNAL( clicked () ), this, SIGNAL(requestRefresh ( )));
  
  /* ======= gateway tab ======= */
  /*TODO, set this in Ui rather*/
  connect(httpRadioButton, SIGNAL(toggled ( bool )), httpWidget, SLOT(setEnabled ( bool )));
  
  connect(applyButton, SIGNAL(released ()), this, SLOT(setGatewayInClient()));
  
  /*gateway settings save*/
  connect(httpRadioButton, SIGNAL(clicked ()), this, SLOT(saveSettings ( )));
  
    /*TODO send port and proxy down*/
  connect(httpProxyLineEdit, SIGNAL(editingFinished ()), this, SLOT(saveGatewaySettings ( )));
  connect(portLineEdit, SIGNAL(editingFinished ()), this, SLOT(saveGatewaySettings ( )));
  
  
  connect(  &mxit, 
            SIGNAL(outgoingVariables(const VariableHash&)), 
            this, 
            SLOT(incomingVariables(const VariableHash&))  );
  
  /* ======= theme tab ======= */
  connect(  themeOpenButton, SIGNAL( released () ), this, SLOT(openThemeBrowser ()));
  
  connect(
            themeComboBox, SIGNAL( currentIndexChanged ( const QString & ) ), 
            this, SLOT(loadTheme(const QString &)));
  connect(
            reloadButton, SIGNAL( released () ), 
            this, SLOT(reloadCurrentTheme()));
            
            
  connect(reloadButton, SIGNAL(released ()), this, SLOT(saveSettings ( )));
  
  /* ======= conversations tab ======= */
  connect(conversationsOpenButton, SIGNAL( released () ), this, SLOT(openConversationsBrowser ()));
  
  connect(directoryLineEdit , SIGNAL(editingFinished ()), this, SLOT (refreshComboBox ())); 
  
  
  
  /*load settings*/
  
  autoLoginCheckBox->setChecked(settings.value("autoLogin").toBool());
  showLoginDialogCheckBox->setChecked(settings.value("showLoginDialogOnNonAutoStart").toBool());
  hideOfflineCheckBox->setChecked(settings.value("hideOfflineContacts").toBool());
  
  setBaseConversationsDirectory(settings.value("baseConversationsDirectory").toString());
  
  setBaseThemeDirectory(settings.value("baseThemeDirectory").toString());

  
  //qDebug() << themeComboBox->findText (settings.value("themeSubDirectory").toString());
  themeComboBox->setCurrentIndex (themeComboBox->findText (settings.value("themeSubDirectory").toString()));  
  
  
  //qDebug() << settings.value("autoLogin").toBool();
  //qDebug() << settings.value("hideOfflineContacts").toBool();
  //qDebug() << settings.value("baseThemeDirectory").toString();
  //qDebug() << settings.value("baseConversationsDirectory").toString();
  //qDebug() << settings.value("themeSubDirectory").toString();
  
  
  loadingSettings = false;
  reloadCurrentTheme();
  
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

  if (!loadingSettings) {
    settings.setValue("autoLogin", isAutoLogin());
    settings.setValue("showLoginDialogOnNonAutoStart", showLoginDialogOnNonAutoStart());
    settings.setValue("hideOfflineContacts", hideOfflineContacts());
    
    saveGatewaySettings();
    saveThemeOptionsSettings();
    
  }
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
bool Options::showLoginDialogOnNonAutoStart() {
  return showLoginDialogCheckBox->isChecked();
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
** Requests the new gateway that should be used to the client 
**
****************************************************************************/


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::incomingVariables(const VariableHash& variables) {
  
  /* notify gateway */
  gatewayComboBox->clear();
  addGateway(variables.value("soc1"));
  addGateway(variables.value("soc2"));
  addGateway(variables.value("http1"));
  addGateway(variables.value("http2"));
  
  if (settings.contains("gateway")) {
    setSelectedGateway(settings.value("gateway").toString());
  }
  else {
    setSelectedGateway(variables.value("soc1"));
  }
  httpProxyLineEdit->setText(settings.value("proxyHost").toString());
  portLineEdit->setText(settings.value("proxyPort").toString());
  usernameLineEdit->setText(settings.value("proxyUsername").toString());
  passwordLineEdit->setText(settings.value("proxyPassword").toString());
  
  saveGatewaySettings();
}

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
void Options::setGatewayInClient () {
  
  mxit.setGateway(gatewayComboBox->currentText (), httpProxyLineEdit->text(), portLineEdit->text().toUInt(), usernameLineEdit->text(), passwordLineEdit->text());
}


/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::saveGatewaySettings() {
  
  settings.setValue("gateway", gatewayComboBox->currentText () );
  settings.setValue("proxyHost", httpProxyLineEdit->text());
  settings.setValue("proxyPort", portLineEdit->text());
  settings.setValue("proxyUsername", usernameLineEdit->text());
  settings.setValue("proxyPassword", passwordLineEdit->text());
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

void Options::saveThemeOptionsSettings() {

  settings.setValue("baseThemeDirectory", getBaseThemeDirectory());
  settings.setValue("baseConversationsDirectory", getBaseConversationsDirectory());
  settings.setValue("themeSubDirectory", themeComboBox->currentText ());

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

QString Options::getBaseConversationsDirectory() {
  return conversationsLineEdit->text();
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

QString Options::getBaseThemeDirectory() {
  return directoryLineEdit->text();
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
    QString selectedText = themeComboBox->currentText ();
    themeComboBox->clear();
    QFileInfoList fil = dir.entryInfoList ( QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    
    Q_FOREACH(const QFileInfo& fileInfo, fil) {
      themeComboBox->addItem (fileInfo.fileName());
    }
    themeComboBox->setCurrentIndex (themeComboBox->findText (selectedText));  
  }
  else {
    //directoryLineEdit->setForground(QBrush(Qt::red));
    themeComboBox->clear();
  }
  saveSettings();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Options::loadTheme(const QString &dir){
  if (!loadingSettings) {
    //qDebug() << "loadTheme " << dir;
    QDir themeDir = getBaseThemeDirectory();
    themeDir.cd(dir);
    theme.load(themeDir);
    
    emit themeChanged();
  }
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::reloadCurrentTheme(){
  loadTheme(themeComboBox->currentText ());
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::reAdd() {
  generalTab->layout()->addWidget(generalWidget);
  themeTab->layout()->addWidget(themeWidget);
  conversationsTab->layout()->addWidget(conversationsWidget);

}

} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */




