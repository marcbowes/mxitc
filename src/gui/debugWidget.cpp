

#include "debugWidget.h"

namespace MXit
{

namespace GUI
{


/****************************************************************************
**
** Author: Richard Baxter
**
** Widget constructor
**
****************************************************************************/
DebugDockWidget::DebugDockWidget(QWidget* parent) : QDockWidget(parent)
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
DebugDockWidget::~DebugDockWidget()
{

}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/

void DebugDockWidget::refreshRequested()
{
  emit requestVariableHashRefresh();
}

/****************************************************************************
**
** Author: Richard Baxter
**
****************************************************************************/
void DebugDockWidget::incomingVariableHash(const VariableHash& variables)
{
  //qDebug() << "DebugDockWidget::incomingVaraibleHash(const VariableHash& variables)";
  
  listWidget->clear();
  
  for (VariableHashConstItr i = variables.constBegin(); i != variables.constEnd(); ++i)
     listWidget->addItem ( i.key() +": " + i.value());
  
}


} /* end of GUI namespace */

} /* end of MXit namespace */




