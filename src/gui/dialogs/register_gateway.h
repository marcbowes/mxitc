#ifndef __MXIT_GUI_DILIOGS_REGISTERGATEWAY_H__
#define __MXIT_GUI_DILIOGS_REGISTERGATEWAY_H__

#include "ui_register_gateway.h"
#include <QDialog>

namespace MXit
{

namespace GUI
{

namespace Dialog
{

class RegisterGateway : public QDialog, private Ui::RegisterGatewayDialog
{
  Q_OBJECT

  public:         /* class specific */
  
  RegisterGateway(const QString& gatewayLabelText, bool onlyShowUsername = false, QWidget *parent = 0);
  ~RegisterGateway();
  
  public: /* methods */
  
  QString getPassword() const;
  QString getUsername() const;
  
  signals:

  private:
  
};

} /* end of Dialog namespace */

} /* end of GUI namespace */

} /* end of MXit namespace */


#endif /* __MXIT_GUI_DILIOGS_REGISTERGATEWAY_H__ */

