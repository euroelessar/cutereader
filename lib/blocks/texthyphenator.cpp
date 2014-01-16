#include "texthyphenator.h"
#include <QStandardPaths>
#include <QTextBoundaryFinder>
#include <QCache>

namespace CuteReader {

TextHyphenDict readDict(const QString &path)
{
    TextHyphenDict result;
    result.maxWordSize = 0;

    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return result;

    QByteArray codec = file.readLine();
    QTextStream in(&file);
    in.setCodec(codec);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line == QStringLiteral("%") || line.isEmpty())
            break;

        QString word;
        QVector<quint8> values;

        quint8 last = 0;
        if (line[line.size() - 1].isDigit()) {
            last = line[line.size() - 1].toLatin1() - '0';
            line.chop(1);
        }

        for (int i = 0; i < line.size(); ++i) {
            if (line[i].isDigit()) {
                values.append(line[i++].toLatin1() - '0');
                word.append(line[i]);
            } else {
                values.append(0);
                word.append(line[i]);
            }
        }

        values.append(last);

        result.data[word] = values;
        result.maxWordSize = qMax(result.maxWordSize, word.size());
    }

    return result;
}

class StringWrapper
{
public:
    StringWrapper(const QStringRef &word)
        : m_word(word), m_values(word.size(), 0)
    {
    }

    int size() const
    {
        return m_word.size() + 2;
    }

    QChar at(int i) const
    {
        if (i == 0 || i == m_word.size() + 1)
            return QLatin1Char('.');
        return m_word.at(i - 1).toLower();
    }

    void fillMid(int from, int length, QString &mid)
    {
        mid.resize(length);

        for (int i = 0; i < length; ++i)
            mid[i] = at(from + i);
    }

    void setValue(int i, quint8 value)
    {
        if (i < 0 || i >= m_values.size())
            return;

        auto &result = m_values[i];

        result = qMax(result, value);
    }

    quint8 value(int i) const
    {
        return m_values.value(i, 0);
    }

private:
    const QStringRef m_word;
    QVector<quint8> m_values;
};

void TextHyphenDict::hyphenate(QString &result, int from, QVector<int> &offsets, const QStringRef &word) const
{
    StringWrapper str(word);
    QString substr;

    for (int length = 1; length <= qMin(maxWordSize, word.size() + 2); ++length) {
        for (int start = 0; start + length <= word.size() + 2; ++start) {
            str.fillMid(start, length, substr);

            auto it = data.find(substr);
            if (it == data.end())
                continue;

            const QVector<quint8> &values = it.value();
            for (int i = 0; i < values.size(); ++i)
                str.setValue(start + i, values[i]);
        }
    }

    for (int i = 0; i < word.size(); ++i) {
        if (i > 1 && i + 2 < word.size()) {
            if (str.value(i + 1) & 1) {
                result.append(QChar::SoftHyphen);
                offsets[from + i] = 1;
            }
        }
        result.append(word.at(i));
    }
}

QCache<QString, TextHyphenDict> hyphenCache;

TextHyphenator::TextHyphenator(const QString &lang)
{
    if (auto dict = hyphenCache.object(lang)) {
        m_dict = *dict;
        return;
    }

    const QString fileName = QStringLiteral("hyphen/hyph_%1.dic").arg(lang);
    const QString filePath = QStandardPaths::locate(QStandardPaths::DataLocation, fileName);
    m_dict = readDict(filePath);
    hyphenCache.insert(lang, new TextHyphenDict(m_dict));

    qDebug() << fileName << filePath << m_dict.data.size();
}

void TextHyphenator::hyphenate(QString &text, QList<FormatRange> &formats) const
{
    QString result;
    QVector<int> offsets(text.size() + 1, 0);

    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text);
    int position = 0;

    while (position != -1) {
        int nextPosition = finder.toNextBoundary();

        if (finder.boundaryReasons() & QTextBoundaryFinder::EndOfItem) {
            m_dict.hyphenate(result, position, offsets, text.midRef(position, nextPosition - position));
        } else {
            result += text.midRef(position, nextPosition - position);
        }
        position = nextPosition;
    }

    for (int i = 1; i < offsets.size(); ++i)
        offsets[i] += offsets[i - 1];

    for (FormatRange &range : formats) {
        int start = range.start;
        int end = range.start + range.length;
        range.start += offsets[start];
        range.length += offsets[end] - offsets[start];
    }

    text = result;
}

} //namespace CuteReader
