#include "qtzlencodingconverterprovider.h"
#include "qtzlencodingconverter.h"

QtZLEncodingConverterProvider::QtZLEncodingConverterProvider()
{
}

bool QtZLEncodingConverterProvider::providesConverter(const std::string &encoding)
{
    QTextCodec *codec = QTextCodec::codecForName(encoding.c_str());
    return codec != nullptr;
}

shared_ptr<ZLEncodingConverter> QtZLEncodingConverterProvider::createConverter(const std::string &encoding)
{
    QTextCodec *codec = QTextCodec::codecForName(encoding.c_str());
    return codec ? new QtZLEncodingConverter(codec) : nullptr;
}
