#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include <QDockWidget>

namespace Ui {
class SystemStatus;
}

class SystemStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SystemStatus(QWidget *parent = 0);
    ~SystemStatus();
    
private:
    Ui::SystemStatus *ui;
};

#endif // SYSTEMSTATUS_H
