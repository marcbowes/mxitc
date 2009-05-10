

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
Options::Options(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
  connect(httpRadioButton, SIGNAL(toggled ( bool )), httpWidget, SLOT(setEnabled ( bool )));
  
  connect(httpRadioButton, SIGNAL(toggled ( bool )), this, SIGNAL(gatewaySelected( bool )));
  
  connect(themeOpenButton, SIGNAL( released () )   , this, SLOT(openThemeBrowser ()));
  
  connect(
            themeComboBox, SIGNAL( currentIndexChanged ( const QString & ) ), 
            this, SLOT(loadTheme(const QString &)));
  
  connect(dirLineEdit , SIGNAL(editingFinished ()), this, SLOT (refreshComboBox ())); 
  
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Options::~Options()
{

}

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
** Author: Richard Baxter
**
****************************************************************************/


QString Options::getSelectedTheme() {
  return themeComboBox->itemText ( themeComboBox->currentIndex () );
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
  return dirLineEdit->text();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Options::setBaseThemeDirectory(const QString& dir) {
    /*TODO check if directory exists, if not, display it in red and return - rax*/
    dirLineEdit->setText(dir);
  
    refreshComboBox();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Options::refreshComboBox ()
{
  QDir dir(dirLineEdit->text());

  if (dir.exists()) {
    //dirLineEdit->setForground(QBrush(Qt::black));
    themeComboBox->clear();
    QFileInfoList fil = dir.entryInfoList ( QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    
    Q_FOREACH(const QFileInfo& fileInfo, fil) {
      themeComboBox->addItem (fileInfo.fileName());
    }
  }
  else {
    //dirLineEdit->setForground(QBrush(Qt::red));
    themeComboBox->clear();
  }
}

void Options::loadTheme(const QString &dir){
  QDir themeDir = getBaseThemeDirectory();
  themeDir.cd(dir);
  theme.load(themeDir);
  
  emit themeChanged();
}

} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */




