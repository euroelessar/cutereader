#ifndef BOOKTEXTSTYLEITEM_H
#define BOOKTEXTSTYLEITEM_H

#include <QQmlPropertyMap>
#include <QTextCharFormat>
#include <QDebug>

class BookTextStyleItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal fontPointSize READ fontPointSize WRITE setFontPointSize NOTIFY fontPointSizeChanged)
public:
    explicit BookTextStyleItem(const QTextCharFormat &format, QObject *parent = 0);

    QTextCharFormat format() const;

    qreal fontPointSize() const
    {
        return m_format.doubleProperty(QTextCharFormat::FontPointSize);
    }

    void setFontPointSize(qreal fontPointSize)
    {
        if (qFuzzyCompare(fontPointSize, m_format.doubleProperty(QTextCharFormat::FontPointSize)))
            return;

        if (qIsNull(fontPointSize))
            m_format.clearProperty(QTextCharFormat::FontPointSize);
        else
            m_format.setProperty(QTextCharFormat::FontPointSize, fontPointSize);

        emit changed();
    }

signals:
    void changed();
    void fontPointSizeChanged(qreal fontPointSize);

private:
    QTextCharFormat m_format;
};

#endif // BOOKTEXTSTYLEITEM_H
