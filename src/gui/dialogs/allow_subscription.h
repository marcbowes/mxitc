/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** MXit::GUI::Dialog::Login provides a login window.
** This is an extension of the generated dialog, made with Designer.
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_ALLOWSUBSCPRIPTION_H__
#define __MXIT_GUI_DIALOG_ALLOWSUBSCPRIPTION_H__

#include "mxit/client.h"
//#include "gui/mxit_dialog.h"

#include "ui_allow_subscription.h"

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class AllowSubscription : public QDialog, private Ui::AllowSubscriptionDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  AllowSubscription(const QString & inviteMessage, const QString & nickname, const QStringList& groups, QWidget *parent = 0);
  ~AllowSubscription();
  
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

