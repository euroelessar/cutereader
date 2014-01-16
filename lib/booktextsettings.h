#ifndef BOOKTEXTSETTINGS_H
#define BOOKTEXTSETTINGS_H

#include "bookblock.h"
#include "config.h"
#include <QReadWriteLock>

namespace CuteReader {

class BookTextSettings : public QObject
{
    Q_OBJECT
    Q_ENUMS(CharFormatType)
    Q_ENUMS(BlockFormatType)
    Q_ENUMS(VerticalAlignment)
public:
    explicit BookTextSettings(QObject *parent = 0);

    enum CharFormatType {
        FontPixelSize = QTextCharFormat::FontPixelSize,
        FontWeight = QTextCharFormat::FontWeight,
        FontItalic = QTextCharFormat::FontItalic,
        FontFamily = QTextCharFormat::FontFamily,
        FontStrikeOut = QTextCharFormat::FontStrikeOut,
        VerticalAlignment = QTextCharFormat::TextVerticalAlignment,
        UnderlineStyle = QTextCharFormat::TextUnderlineStyle
    };

    enum BlockFormatType {
        Alignment = QTextBlockFormat::BlockAlignment,
        TopMargin = QTextBlockFormat::BlockTopMargin,
        BottomMargin = QTextBlockFormat::BlockBottomMargin,
        LeftMargin = QTextBlockFormat::BlockLeftMargin,
        RightMargin = QTextBlockFormat::BlockRightMargin,
        TextIndent = QTextBlockFormat::TextIndent,
        NonBreakableLines = QTextBlockFormat::BlockNonBreakableLines
    };

    // Keep in sync with QTextCharFormat
    enum VerticalAlignment {
        AlignNormal = 0,
        AlignSuperScript,
        AlignSubScript,
        AlignMiddle,
        AlignTop,
        AlignBottom,
        AlignBaseline
    };

    BookStyle style() const;

signals:
    void changed(const BookStyle &style);

private:
    Config m_config;
    mutable QReadWriteLock m_lock;
    BookStyle m_style;
};

} //namespace CuteReader

#endif // BOOKTEXTSETTINGS_H
