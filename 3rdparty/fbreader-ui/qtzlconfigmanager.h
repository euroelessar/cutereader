#ifndef QTZLCONFIGMANAGER_H
#define QTZLCONFIGMANAGER_H

#include "../fbreader/zlibrary/core/src/options/ZLConfig.h"

class QtZLConfigManager : public ZLConfigManager
{
public:
    QtZLConfigManager();

    static void createInstance();

    ZLConfig *createConfig() const;
};

#endif // QTZLCONFIGMANAGER_H
