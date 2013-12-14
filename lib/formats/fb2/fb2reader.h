#ifndef FB2READER_H
#define FB2READER_H

#include "../../bookblock.h"
#include "../../bookinfo.h"
#include <QMimeType>
#include <QTextCharFormat>

class QXmlStreamReader;

class FB2Reader
{
public:
    enum Flag {
        Info        = 0x01,
        Text        = 0x02,
        ImageSizes  = 0x04,
        All         = 0xff
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    FB2Reader();

    bool canRead(const QMimeType &mimeType) const;
    BookInfo read(const QUrl &source, QIODevice *device, Flags flags);

private:
    struct ImageInfo
    {
        QUrl source;

        BookBlockFactory::Ptr toFactory();
    };

    void readDescription(QXmlStreamReader &in, BookInfo &info, const QUrl &baseUrl);
    BookBlockFactory::Ptr readParagraph(QXmlStreamReader &in, const QList<QTextCharFormat> &baseFormats);
    ImageInfo readImage(QXmlStreamReader &in, const QUrl &baseUrl);
    BodyInfo readBody(QXmlStreamReader &in, const QUrl &baseUrl);

    struct FormatDescription
    {
        QString name;
        std::function<void (const QXmlStreamReader &in, QTextCharFormat &)> change;
    };

    QList<FormatDescription> m_descriptions;
};

#endif // FB2READER_H
