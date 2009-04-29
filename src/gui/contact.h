/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
** Author: Richard Baxter, 2009
**
** Abstract class that stores all data relavant to a single contact
**
****************************************************************************/

#include <QObject>

namespace MXit
{


class Contact : public QObject
{
  Q_OBJECT

  public:         /* class specific */
	
  Contact(const QString& name = "", const QString& cellphone = "");
  ~Contact();

  
  public:         /* methods */

  QString getName();
  QString getCellphone();

  private: /* variables */
  QString name;
  QString cellphone;


};


} /* end namespace MXit */







