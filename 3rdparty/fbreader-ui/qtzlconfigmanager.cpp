#include "qtzlconfigmanager.h"
#include "qtzlconfig.h"

QtZLConfigManager::QtZLConfigManager()
{
}

void QtZLConfigManager::createInstance()
{
    ourInstance = new QtZLConfigManager;
}


ZLConfig *QtZLConfigManager::createConfig() const
{
    ourIsInitialised = true;
    return new QtZLConfig;
}
