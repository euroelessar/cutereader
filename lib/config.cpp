#include "config.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QVariantHash>
#include <QMetaProperty>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QVector>
#include <QPair>
#include <QSet>
#include <QSaveFile>

#define CONFIG_PATH QStringLiteral("/cutereader")
#define CONFIG_FILE QStringLiteral("/cutereader/config.json")

static void populateConfig(QVariantHash &data, QString &prefix, const QJsonObject &object)
{
    if (!prefix.isEmpty())
        prefix += QLatin1Char('.');
    const auto size = prefix.size();

    for (auto it = object.begin(); it != object.end(); ++it) {
        prefix += it.key();
        if (it.value().isObject()) {
            populateConfig(data, prefix, it.value().toObject());
        } else {
            data.insert(prefix, it.value().toVariant());
        }
        prefix.resize(size);
    }
}

static QVariantHash loadConfig(const QString &path)
{
    if (path.isEmpty())
        return QVariantHash();

    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qWarning("Can't access \"%s\"", qPrintable(file.fileName()));
        return QVariantHash();
    }
    auto document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject()) {
        qWarning("File \"%s\" is not valid json object", qPrintable(file.fileName()));
        return QVariantHash();
    }

    QVariantHash data;
    QString prefix;
    populateConfig(data, prefix, document.object());
    return data;
}

static QVariantHash loadConfig()
{
    const auto path = QStandardPaths::locate(QStandardPaths::ConfigLocation, CONFIG_FILE);
    return loadConfig(path);
}

static void saveConfig(const QVariantHash &config)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + CONFIG_FILE;

    QSaveFile file(path);
    if (!file.open(QFile::WriteOnly)) {
        qWarning("Can't access \"%s\"", qPrintable(file.fileName()));
        return;
    }

    QVector<QPair<QString, QVariant>> entries;
    entries.reserve(config.size());
    for (auto it = config.begin(); it != config.end(); ++it)
        entries << qMakePair(it.key(), it.value());
    qSort(entries.begin(), entries.end(),
          [] (const QPair<QString, QVariant> &first, const QPair<QString, QVariant> &second) {
        return first.first < second.first;
    });

    QList<QJsonObject> objectStack;
    QStringList pathStack;

    objectStack << QJsonObject();

    for (auto &entry : entries) {
        const auto paths = entry.first.split(QLatin1Char('.'));
        int lastIndex;
        for (lastIndex = 0; lastIndex < qMin(paths.size() - 1, pathStack.size()); ++lastIndex) {
            if (pathStack[lastIndex] != paths[lastIndex])
                break;
        }
        while (pathStack.size() > lastIndex) {
            auto object = objectStack.takeLast();
            objectStack.last().insert(pathStack.takeLast(), object);
        }
        for (int i = lastIndex; i + 1 < paths.size(); ++i) {
            objectStack << QJsonObject();
            pathStack << paths[i];
        }
        objectStack.last().insert(paths.last(), QJsonValue::fromVariant(entry.second));
    }
    while (!pathStack.isEmpty()) {
        auto object = objectStack.takeLast();
        objectStack.last().insert(pathStack.takeLast(), object);
    }

    auto object = objectStack.last();
    auto json = QJsonDocument(object).toJson();
    file.write(json);
    file.commit();
}

class ConfigData
{
public:
    ConfigData()
    {
        path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + CONFIG_FILE;
        auto dirPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + CONFIG_PATH;

        config = loadConfig();
        lastModified = QFileInfo(path).lastModified();

        if (!QFileInfo(dirPath).exists())
            QDir().mkpath(dirPath);

        watcher.addPath(dirPath);
        QObject::connect(&watcher, &QFileSystemWatcher::directoryChanged,
                         [this] (const QString &dirPath) {
            if (!QFileInfo(dirPath).exists()) {
                QDir().mkpath(dirPath);
                watcher.addPath(dirPath);
            }

            auto date = QFileInfo(path).lastModified();
            if (date != lastModified) {
                lastModified = date;
                updateConfig();
            }
        });

        timer.setInterval(500);
        timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout, [this] () {
            saveConfig(config);
            lastModified = QFileInfo(path).lastModified();
        });
    }

    void updateConfig()
    {
        auto tmpConfig = loadConfig();
        qSwap(config, tmpConfig);
        for (auto it = config.begin(); it != config.end(); ++it) {
            if (it.value() != tmpConfig.value(it.key())) {
                for (auto object : configObjects)
                    object->onValueChanged(it.key(), it.value());
            }
        }
    }

    void updateConfig(const QString &name, const QVariant &newValue)
    {
        QVariant &value = config[name];
        if (value != newValue) {
            value = newValue;
            for (auto object : configObjects)
                object->onValueChanged(name, value);

            if (!timer.isActive())
                timer.start();
        }
    }

    QString path;
    QFileSystemWatcher watcher;
    QVariantHash config;
    QDateTime lastModified;
    QSet<Config*> configObjects;
    QTimer timer;
};

static QWeakPointer<ConfigData> weakConfig;

Config::Config(QObject *parent)
    : QObject(parent)
{
    if (weakConfig) {
        m_data = weakConfig.toStrongRef();
    } else {
        m_data.reset(new ConfigData);
        weakConfig = m_data.toWeakRef();
    }
    m_data->configObjects.insert(this);
}

Config::~Config()
{
    m_data->configObjects.remove(this);
}

QString Config::path() const
{
    return m_path;
}

void Config::setPath(const QString &path)
{
    if (m_path != path) {
        m_path = path;
        emit pathChanged(path);
    }
}

QVariant Config::value(const QString &name)
{
    return m_data->config.value(name);
}

void Config::setValue(const QString &name, const QVariant &value)
{
    m_data->updateConfig(name, value);
}

void Config::loadDefaultConfig(const QString &path)
{
    QVariantHash data = loadConfig(path);

    for (auto it = data.begin(); it != data.end(); ++it) {
        if (m_data->config.contains(it.key()))
            continue;
        m_data->config.insert(it.key(), it.value());
    }
}

void Config::classBegin()
{
}

void Config::componentComplete()
{
    auto meta = metaObject();
    auto configMeta = &Config::staticMetaObject;

    const int slotIndex = ConfigEntry::staticMetaObject.indexOfSlot("onPropertyChanged()");
    Q_ASSERT(slotIndex >= 0);
    const QMetaMethod slot = ConfigEntry::staticMetaObject.method(slotIndex);

    const int startPropertyIndex = configMeta->propertyCount();

    for (int i = startPropertyIndex; i < meta->propertyCount(); ++i) {
        auto property = meta->property(i);
        auto name = m_path + '.' + property.name();

        auto it = m_data->config.find(name);
        if (it != m_data->config.end()) {
            property.write(this, it.value());
        } else {
            m_data->config.insert(name, property.read(this));
        }

        m_properties[name] = property;

        ConfigEntry *entry = new ConfigEntry(this);
        connect(this, property.notifySignal(), entry, slot);
        connect(entry, &ConfigEntry::propertyChanged, [this, property, name] () {
            m_data->updateConfig(name, property.read(this));
        });
    }
}

void Config::onValueChanged(const QString &key, const QVariant &value)
{
    auto it = m_properties.find(key);
    if (it != m_properties.end())
        it.value().write(this, value);
}


ConfigEntry::ConfigEntry(Config *parent)
    : QObject(parent)
{
}

void ConfigEntry::onPropertyChanged()
{
    emit propertyChanged();
}
