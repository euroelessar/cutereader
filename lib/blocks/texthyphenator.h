#ifndef TEXTHYPHENATOR_H
#define TEXTHYPHENATOR_H

#include "booktextblockfactory.h"

struct TextHyphenDict
{
    QHash<QString, QVector<quint8>> data;
    int maxWordSize;

    void hyphenate(QString &result, int from, QVector<int> &offsets, const QStringRef &word) const;
};

class TextHyphenator
{
public:
    TextHyphenator(const QString &lang);

    void hyphenate(QString &text, QList<FormatRange> &formats) const;

private:
    TextHyphenDict m_dict;
};

#endif // TEXTHYPHENATOR_H
