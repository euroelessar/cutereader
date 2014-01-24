#ifndef QTZLENCODINGCONVERTERPROVIDER_H
#define QTZLENCODINGCONVERTERPROVIDER_H

#include "../fbreader/zlibrary/core/src/encoding/ZLEncodingConverterProvider.h"

class QtZLEncodingConverterProvider : public ZLEncodingConverterProvider
{
public:
    QtZLEncodingConverterProvider();

    bool providesConverter(const std::string &encoding);
    shared_ptr<ZLEncodingConverter> createConverter(const std::string &encoding);
};

#endif // QTZLENCODINGCONVERTERPROVIDER_H
