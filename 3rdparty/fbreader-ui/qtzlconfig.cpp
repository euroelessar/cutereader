#include "qtzlconfig.h"
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

QtZLConfig::QtZLConfig() :
    m_globalSettings(new QSettings(QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName())),
    m_settings(new QSettings(QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName()))
{
}


void QtZLConfig::listOptionNames(const std::string &groupName, std::vector<std::string> &names)
{
    m_settings->beginGroup(QString::fromStdString(groupName));

    for (const auto &key : m_settings->childKeys())
        names.push_back(key.toStdString());

    m_settings->endGroup();
}

void QtZLConfig::listOptionGroups(std::vector<std::string> &groups)
{
    for (const auto &group : m_settings->childGroups())
        groups.push_back(group.toStdString());
}

void QtZLConfig::removeGroup(const std::string &name)
{
    m_settings->remove(QString::fromStdString(name));
}

const std::string &QtZLConfig::getDefaultValue(const std::string &group, const std::string &name, const std::string &defaultValue) const
{
    m_globalSettings->beginGroup(QString::fromStdString(group));
    m_tmp = m_globalSettings->value(QString::fromStdString(name), QString::fromStdString(defaultValue)).toString().toStdString();
    m_globalSettings->endGroup();

    return m_tmp;
}

const std::string &QtZLConfig::getValue(const std::string &group, const std::string &name, const std::string &defaultValue) const
{
    m_settings->beginGroup(QString::fromStdString(group));
    m_tmp = m_settings->value(QString::fromStdString(name), QString::fromStdString(defaultValue)).toString().toStdString();
    m_settings->endGroup();

    return m_tmp;
}

void QtZLConfig::setValue(const std::string &group, const std::string &name, const std::string &value, const std::string &category)
{
    (void) category;

    m_settings->beginGroup(QString::fromStdString(group));
    m_settings->setValue(QString::fromStdString(name), QString::fromStdString(value));
    m_settings->endGroup();
}

void QtZLConfig::unsetValue(const std::string &group, const std::string &name)
{
    m_settings->beginGroup(QString::fromStdString(group));
    m_settings->remove(QString::fromStdString(name));
    m_settings->endGroup();
}

bool QtZLConfig::isAutoSavingSupported() const
{
    return true;
}

void QtZLConfig::startAutoSave(int seconds)
{
    (void) seconds;
}
