#ifndef BOOKTEXTSTYLEITEM_H
#define BOOKTEXTSTYLEITEM_H

#include <QQmlPropertyMap>
#include <QTextCharFormat>
#include <QDebug>

#define FORMAT_PROPERTY(NAME, FORMAT) \
    Q_PROPERTY(QVariant NAME READ NAME WRITE set ## NAME NOTIFY NAME ## Changed) \
public: \
    QVariant NAME() const \
    { \
        return m_format.property(FORMAT); \
    } \
 \
    void set ## NAME(const QVariant &arg) \
    { \
        QVariant realArg = updateValue(FORMAT, arg); \
        emit NAME ## Changed(); \
    } \
private:

class BookTextStyleItem : public QObject
{
    Q_OBJECT
    FORMAT_PROPERTY(fontPointSize, QTextCharFormat::FontPointSize)
    FORMAT_PROPERTY(fontWeight, QTextCharFormat::FontWeight)
    FORMAT_PROPERTY(fontItalic, QTextCharFormat::FontItalic)
    FORMAT_PROPERTY(fontFamily, QTextCharFormat::FontFamily)
public:
    explicit BookTextStyleItem(const QTextCharFormat &format, QObject *parent = 0);

    QTextCharFormat format() const;

signals:
    void changed();
    void fontPointSizeChanged();
    void fontWeightChanged();
    void fontItalicChanged();
    void fontFamilyChanged();

protected:
    QVariant updateValue(QTextCharFormat::Property key, const QVariant &input);

private:
    QTextCharFormat m_format;
};

#undef PROPERTY_REAL

#endif // BOOKTEXTSTYLEITEM_H
