/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
**
****************************************************************************/

#ifndef __MXIT_GUI_DIALOG_STARTGROUPCHAT_H__
#define __MXIT_GUI_DIALOG_STARTGROUPCHAT_H__


#include "ui_start_group_chat.h"

#include <QString>

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class StartGroupChat : public QDialog, private Ui::StartGroupChatDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  StartGroupChat(QWidget *parent = 0);
  ~StartGroupChat();
  
  public: /* methods */
  
  void setText(const QString& text);
  QString getRoomname() const;
  
  signals:

  private:
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */
 
#endif

