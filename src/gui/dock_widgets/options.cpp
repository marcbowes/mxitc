

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
void Options::openThemeBrowser ()
{
  QFileDialog fileDiag ( this, "Choose your theme directory");
  fileDiag.setFileMode(QFileDialog::DirectoryOnly);
  if (fileDiag.exec ()){
    QDir selection = fileDiag.selectedFiles ().front();
    
    /*TODO check if directory exists, if not, display it in red and return - rax*/
    /*TODO probably do in a different slot - rax*/
    dirLineEdit->setText(selection.absolutePath ());
  
  
    refreshComboBox();
  }
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
    themeComboBox->clear();
    QFileInfoList fil = dir.entryInfoList ( QStringList(), QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name );
    
    Q_FOREACH(const QFileInfo& fileInfo, fil) {
      themeComboBox->addItem (fileInfo.fileName());
    }
  }
}

void Options::loadTheme(const QString & dir){

  theme.load(QDir(dirLineEdit->text()+"/"+dir));
}

} /* end of DockWidget namespace */


} /* end of GUI namespace */

} /* end of MXit namespace */




