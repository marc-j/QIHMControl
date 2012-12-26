#ifndef IHMCORE_H
#define IHMCORE_H

#include <QApplication>
#include "ui/IHMControl.h"
#include "UAV.h"
#include "protocol/Protocol.h"

class IHMCore : public QApplication
{
    Q_OBJECT

public:
    IHMCore(int &argc, char* argv[]);
    ~IHMCore();

private:
    IHMControl* mainWindow;
};
#endif // IHMCORE_H
