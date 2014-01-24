#include "qtzlencodingconverter.h"

QtZLEncodingConverter::QtZLEncodingConverter(QTextCodec *codec) : m_codec(codec)
{
    reset();
}

void QtZLEncodingConverter::convert(std::string &dst, const char *srcStart, const char *srcEnd)
{
    const auto len = srcEnd - srcStart;
    QByteArray tmp = m_decoder->toUnicode(srcStart, len).toUtf8();
    dst.append(tmp.begin(), tmp.end());
}

void QtZLEncodingConverter::reset()
{
    m_decoder.reset(m_codec->makeDecoder());
}

bool QtZLEncodingConverter::fillTable(int *map)
{
    (void) map;
    return false;
}
