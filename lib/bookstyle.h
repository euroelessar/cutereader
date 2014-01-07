#ifndef BOOKSTYLE_H
#define BOOKSTYLE_H

#include <QObject>
#include <QReadWriteLock>
#include "bookblock.h"

#define COLOR_PROPERTY(NAME) \
    Q_PROPERTY(QColor NAME READ NAME WRITE set ## NAME NOTIFY changed) \
public: \
    QColor NAME() const \
    { \
        return value(QStringLiteral(#NAME)); \
    } \
 \
    void set ## NAME(const QColor &arg) \
    { \
        updateValue(QStringLiteral(#NAME), arg); \
    } \
private:

class BookStyleItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(GenericColorType)
    Q_ENUMS(ForegroundColorType)
    COLOR_PROPERTY(background)
    COLOR_PROPERTY(base)
    COLOR_PROPERTY(standard)
    COLOR_PROPERTY(title)
    COLOR_PROPERTY(internalAnchor)
    COLOR_PROPERTY(noteAnchor)
    COLOR_PROPERTY(externalAnchor)
public:
    enum GenericColorType {
        Background = 0
    };

    enum ForegroundColorType {
        Base = Background + 1,
        Standard,
        Title,
        InternalAnchor,
        NoteAnchor,
        ExternalAnchor
    };

    enum {
        TotalCount = ExternalAnchor + 1
    };

    explicit BookStyleItem(QObject *parent = 0);

    QVector<QColor> colors() const;

    static int indexForFormat(Format::Type type);

protected:
    QColor value(const QString &name) const;
    void updateValue(const QString &name, const QColor &arg);

signals:
    void changed();

private:
    QColor *colorByName(const QString &name) const;

    mutable QVector<QColor> m_formats;
    mutable QReadWriteLock m_lock;
};

#undef PROPERTY_TEXT_STYLE

#endif // BOOKSTYLE_H
