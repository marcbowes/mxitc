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


namespace MXit
{

namespace GUI
{

class MXitDockWidget : public QDockWidget
{
  Q_OBJECT

  public:         /* class specific */
  
  MXitDockWidget(QWidget *parent = 0);
  virtual ~MXitDockWidget();

  private: /* methods */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  public slots:
  void toggleVisibility();
  

  protected slots:
  
  private slots:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  protected:        /* variables */
  
};

}

}
 
#endif
