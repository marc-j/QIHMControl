#ifndef SERIALCONFIGURATION_H
#define SERIALCONFIGURATION_H

#include <QWidget>

#include <protocol/Protocol.h>

namespace Ui {
class SerialConfiguration;
}

class SerialConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit SerialConfiguration(QWidget *parent = 0);
    ~SerialConfiguration();

protected:
    Protocol* protocol;

private:
    Ui::SerialConfiguration *ui;

protected slots:
    void updateValues();

signals:
    void serialSettingsChanged(QString portName, BaudRateType baud, DataBitsType bit,  StopBitsType stop, ParityType parity);
};

#endif // SERIALCONFIGURATION_H
