/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::MXitDockWindow is a generalisd QDockWindow for the PC MXit client
**
****************************************************************************/

#ifndef __MXIT_GUI_MXIT_DOCKWIDGET_H__
#define __MXIT_GUI_MXIT_DOCKWIDGET_H__

#include <QDockWidget>
#include <QMoveEvent>

#include "theme.h"

namespace MXit
{

namespace GUI
{

class MXitDockWidget : public QDockWidget
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitDockWidget(QWidget *parent, Theme &theme);
  virtual ~MXitDockWidget();
  
  signals:
  
  void sendLog(const QString& message);
  void moved();
  
  //void requestRefresh();

  private: /* methods */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 

  public slots:
  void toggleVisibility(); /*FIXME there is apparently something that does this already in Qt... find it - rax*/
  
  void moveEvent ( QMoveEvent * event );

  protected slots:
  
  private slots:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  protected:        /* variables */
  
  Theme &theme;
  
};

}

}
 
#endif

