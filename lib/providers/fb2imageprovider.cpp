#include "fb2imageprovider.h"
#include <QDebug>
#include <QImageReader>
#include <QBuffer>
#include <QElapsedTimer>
#include <QXmlStreamReader>
#include <QFile>
#include "../archivereader.h"

FB2ImageProvider::FB2ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image,
                          QQuickImageProvider::ForceAsynchronousImageLoading)
{
}


QImage FB2ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);

    QString imageId = id.section(QLatin1Char('#'), 1);
    QString imagePath = QString::fromUtf8(QByteArray::fromHex(id.section(QLatin1Char('#'), 0, 0).toLatin1()));

    ArchiveReader reader(imagePath);
    if (!reader.open())
        return QImage();

    QXmlStreamReader in(reader.device());

    bool inBinary = false;

    while (in.readNext()) {
        if (in.tokenType() == QXmlStreamReader::Invalid)
            break;
        switch (in.tokenType()) {
        case QXmlStreamReader::StartElement:
            if (in.name() == QStringLiteral("binary")
                    && in.attributes().value(QStringLiteral("id")) == imageId) {
                inBinary = true;
            }
            break;
        case QXmlStreamReader::EndElement:
            inBinary = false;
            break;
        case QXmlStreamReader::Characters:
            if (inBinary) {
                QByteArray binary = QByteArray::fromBase64(in.text().toLatin1());
                QBuffer buffer(&binary);
                buffer.open(QIODevice::ReadOnly);
                QImageReader reader(&buffer);

                if (size)
                    *size = reader.size();

                return reader.read();
            }
            break;
        default:
            break;
        }
    }

    return QImage();
}
