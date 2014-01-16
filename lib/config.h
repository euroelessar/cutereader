#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QMetaProperty>
#include <QQmlParserStatus>
#include <QSharedPointer>

namespace CuteReader {

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

    Q_INVOKABLE QVariant value(const QString &name);
    Q_INVOKABLE bool hasValue(const QString &name);
    Q_INVOKABLE void setValue(const QString &name, const QVariant &value);

    void loadDefaultConfig(const QString &path);

    virtual void classBegin();
    virtual void componentComplete();

signals:
    void pathChanged(const QString &path);
    void valueChanged(const QString &key, const QVariant &value);

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

} //namespace CuteReader

#endif // CONFIG_H
