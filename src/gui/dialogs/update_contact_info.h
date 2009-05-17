/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_UPDATECONTACTINFO_H__
#define __MXIT_GUI_DIALOG_UPDATECONTACTINFO_H__

#include "mxit/client.h"
//#include "gui/mxit_dialog.h"

#include "ui_update_contact_info.h"

#include <QString>

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class UpdateContactInfo : public QDialog, private Ui::UpdateContactInfoDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  UpdateContactInfo(const QString & nickname, const QStringList& groups, const QString& currentGroup, QWidget *parent = 0);
  ~UpdateContactInfo();
  
  public: /* methods */
  
  QString getGroup() const;
  QString getNickname() const;
  
  signals:

  private:
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif

