#include "IHMCore.h"

IHMCore::IHMCore(int &argc, char* argv[]) : QApplication(argc, argv)
{
    // Set application name
    this->setApplicationName("IHMControl");
    this->setApplicationVersion("0.1");
    this->setOrganizationName(QLatin1String("IHMControl"));
    this->setOrganizationDomain("fr.alienxarea");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    mainWindow = IHMControl::instance();
    mainWindow->show();
}

IHMCore::~IHMCore()
{
    mainWindow->close();

    delete UAV::instance();
    delete Protocol::instance();
    delete IHMControl::instance();
}
