#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QMetaProperty>
#include <QQmlParserStatus>
#include <QSharedPointer>

class ConfigData;

class Config : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit Config(QObject *parent = 0);
    ~Config();

    QString path() const;
    void setPath(const QString &path);

    QVariant value(const QString &name);
    void setValue(const QString &name, const QVariant &value);

    void loadDefaultConfig(const QString &path);

    virtual void classBegin();
    virtual void componentComplete();

signals:
    void pathChanged(const QString &path);

private:
    void onValueChanged(const QString &key, const QVariant &value);

    QString m_path;
    QHash<QString, QMetaProperty> m_properties;
    QSharedPointer<ConfigData> m_data;

    friend class ConfigData;
};

class ConfigEntry : public QObject
{
    Q_OBJECT
public:
    ConfigEntry(Config *parent);

public slots:
    void onPropertyChanged();

signals:
    void propertyChanged();
};

#endif // CONFIG_H
