#ifndef BOOKSTYLE_H
#define BOOKSTYLE_H

#include <QObject>
#include <QReadWriteLock>
#include "booktextstyleitem.h"
#include "bookblock.h"
#include "config.h"

#define PROPERTY_TEXT_STYLE(NAME, FORMAT) \
        Q_PROPERTY(BookTextStyleItem *NAME READ NAME CONSTANT FINAL) \
    public: \
        BookTextStyleItem *NAME() const { return m_formats[Format::FORMAT]; } \
    private:

class BookStyleItem : public QObject
{
    Q_OBJECT
    PROPERTY_TEXT_STYLE(base, Base)
    PROPERTY_TEXT_STYLE(standard, Standard)
    PROPERTY_TEXT_STYLE(strong, Strong)
    PROPERTY_TEXT_STYLE(emphasis, Emphasis)
    PROPERTY_TEXT_STYLE(strikeThrough, StrikeThrough)
    PROPERTY_TEXT_STYLE(sub, Sub)
    PROPERTY_TEXT_STYLE(sup, Sup)
    PROPERTY_TEXT_STYLE(title, Title)
    PROPERTY_TEXT_STYLE(internalAnchor, InternalAnchor)
    PROPERTY_TEXT_STYLE(noteAnchor, NoteAnchor)
    PROPERTY_TEXT_STYLE(externalAnchor, ExternalAnchor)
public:
    explicit BookStyleItem(QObject *parent = 0);

    BookStyle style() const;

    void componentComplete();

signals:
    void changed(const BookStyle &style);

private:
    Config m_config;
    QVector<BookTextStyleItem *> m_formats;
    BookStyle m_style;
    mutable QReadWriteLock m_lock;
};

#undef PROPERTY_TEXT_STYLE

#endif // BOOKSTYLE_H
