#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QByteArray>

#include "protocol/Protocol.h"

namespace Ui {
class Console;
}

class Console : public QWidget
{
    Q_OBJECT
    
public:
    explicit Console(QWidget *parent = 0);
    ~Console();

private slots:
    void serialOpened();
    void datasReceive(QByteArray* datas);
    void receiveByte(const unsigned char byte);

protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    
private:
    Ui::Console *ui;

    Protocol* protocol;
};

#endif // CONSOLE_H
