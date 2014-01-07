#ifndef BOOKTEXTSTYLEITEM_H
#define BOOKTEXTSTYLEITEM_H

#include <QQmlPropertyMap>
#include <QTextCharFormat>
#include <QDebug>

#define FORMAT_PROPERTY(NAME, FORMAT) \
    Q_PROPERTY(QVariant NAME READ NAME NOTIFY changed) \
public: \
    QVariant NAME() const \
    { \
        return value(FORMAT); \
    } \
 \
    void set ## NAME(const QVariant &arg) \
    { \
        updateValue(FORMAT, arg); \
    } \
private:

class BookTextStyleItem : public QObject
{
    Q_OBJECT
    FORMAT_PROPERTY(foreground, QTextCharFormat::ForegroundBrush)
    FORMAT_PROPERTY(fontPixelSize, QTextCharFormat::FontPixelSize)
    FORMAT_PROPERTY(fontWeight, QTextCharFormat::FontWeight)
    FORMAT_PROPERTY(fontItalic, QTextCharFormat::FontItalic)
    FORMAT_PROPERTY(fontFamily, QTextCharFormat::FontFamily)
public:
    explicit BookTextStyleItem(const QTextCharFormat &format, QObject *parent = 0);

    QTextCharFormat format() const;

signals:
    void changed();

protected:
    QVariant updateValue(QTextCharFormat::Property key, const QVariant &input);
    QVariant value(QTextCharFormat::Property key) const;

private:
    QTextCharFormat m_format;
};

#undef PROPERTY_REAL

#endif // BOOKTEXTSTYLEITEM_H
