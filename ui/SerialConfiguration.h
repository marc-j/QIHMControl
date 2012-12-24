#ifndef SERIALCONFIGURATION_H
#define SERIALCONFIGURATION_H

#include <QWidget>

namespace Ui {
class SerialConfiguration;
}

class SerialConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit SerialConfiguration(QWidget *parent = 0);
    ~SerialConfiguration();
    
private:
    Ui::SerialConfiguration *ui;
};

#endif // SERIALCONFIGURATION_H
