#include "bookinfoitem.h"

BookInfoItem::BookInfoItem(QObject *parent) :
    QObject(parent)
{
}

QString BookInfoItem::title() const
{
    return m_title;
}

QUrl BookInfoItem::cover() const
{
    return m_cover;
}

void BookInfoItem::setBookInfo(const BookInfo &info)
{
    if (m_title != info.title) {
        m_title = info.title;
        emit titleChanged(m_title);
    }

    if (m_cover != info.cover) {
        m_cover = info.cover;
        emit coverChanged(m_cover);
    }
}
