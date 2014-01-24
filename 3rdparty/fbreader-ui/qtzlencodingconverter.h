#ifndef QTZLENCODINGCONVERTER_H
#define QTZLENCODINGCONVERTER_H

#include <ZLEncodingConverter.h>
#include <QTextCodec>
#include <QScopedPointer>

class QtZLEncodingConverter : public ZLEncodingConverter
{
public:
    QtZLEncodingConverter(QTextCodec *codec);

    void convert(std::string &dst, const char *srcStart, const char *srcEnd);
    void reset();
    bool fillTable(int *map);

private:
    QTextCodec *m_codec;
    QScopedPointer<QTextDecoder> m_decoder;

};

#endif // QTZLENCODINGCONVERTER_H
