#ifndef BOOKTEXTFRAGMENT_H
#define BOOKTEXTFRAGMENT_H

#include <QString>
#include <QList>
#include <QFont>

class BookTextBlock;

class BookTextFragment
{
public:
    BookTextFragment();
    BookTextFragment(const QFont &font, const QStringRef &text);
    
    const QFont &font() const { return m_font; }
    const QStringRef &text() const { return m_text; }
    BookTextFragment mid(int offset, int size = -1);
    
private:
    QFont m_font;
    QStringRef m_text;
    friend class BookTextBlock;
};

#endif // BOOKTEXTFRAGMENT_H
