

#include "debug.h"

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
Debug::Debug(QWidget* parent, Theme &theme) : MXitDockWidget(parent, theme)
{
  setupUi(this);
  
  connect(refreshButton, SIGNAL(released ()), this, SIGNAL (requestVariableHashRefresh()));
  
}



/****************************************************************************
**
** Author: Richard Baxter
**
** Widget destructor
**
****************************************************************************/
Debug::~Debug()
{

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void Debug::refreshRequested()
{
  emit requestVariableHashRefresh();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void Debug::incomingVariableHash(const VariableHash& variables)
{
  //qDebug() << "DebugDockWidget::incomingVaraibleHash(const VariableHash& variables)";
  
  listWidget->clear();
  
  for (VariableHashConstItr i = variables.constBegin(); i != variables.constEnd(); ++i)
     listWidget->addItem ( i.key() +": " + i.value());
  
}


} /* end of DockWidget namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */




