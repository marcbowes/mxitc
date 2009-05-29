/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_UPDATEGROUPNAME_H__
#define __MXIT_GUI_DIALOG_UPDATEGROUPNAME_H__

#include "mxit/client.h"
//#include "gui/mxit_dialog.h"

#include "ui_update_group_name.h"

#include <QString>

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class UpdateGroupName : public QDialog, private Ui::UpdateGroupNameDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  UpdateGroupName(const QStringList& groups, const QString& currentGroup, QWidget *parent = 0);
  ~UpdateGroupName();
  
  public: /* methods */
  
  void setText(const QString& text);
  QString getGroup() const;
  
  signals:

  private:
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif

