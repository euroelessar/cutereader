#ifndef QTZLCONFIG_H
#define QTZLCONFIG_H

#include "../fbreader/zlibrary/core/src/options/ZLConfig.h"
#include <QSettings>

class QtZLConfig : public ZLConfig
{
public:
    QtZLConfig();

    void listOptionNames(const std::string &groupName, std::vector<std::string> &names);
    void listOptionGroups(std::vector<std::string> &groups);
    void removeGroup(const std::string &name);
    const std::string &getDefaultValue(const std::string &group, const std::string &name, const std::string &defaultValue) const;
    const std::string &getValue(const std::string &group, const std::string &name, const std::string &defaultValue) const;
    void setValue(const std::string &group, const std::string &name, const std::string &value, const std::string &category);
    void unsetValue(const std::string &group, const std::string &name);
    bool isAutoSavingSupported() const;
    void startAutoSave(int seconds);

private:
    QScopedPointer<QSettings> m_globalSettings;
    QScopedPointer<QSettings> m_settings;
    mutable std::string m_tmp;
};

#endif // QTZLCONFIG_H
