#include "bookpageiterator.h"

static BookTextPosition createPosition(int body, int block, int blockPosition)
{
    return {
        body,
        block,
        blockPosition
    };
}

static BookTextPosition invalidPosition()
{
    return {
        -1,
        -1,
        -1
    };
}

static inline bool fuzzyEqual(qreal first, qreal second)
{
    return qFuzzyCompare(first, second);
}

static inline bool fuzzyLess(qreal first, qreal second)
{
    return first < second && !fuzzyEqual(first, second);
}

static inline bool fuzzyLessOrEqual(qreal first, qreal second)
{
    return first < second || fuzzyEqual(first, second);
}

BookPageIterator::BookPageIterator(const BookPageItem *page)
    : m_position(page->m_positionValue),
      m_blocks(page->book()->blocks(m_position.body)),
      m_size(page->width(), page->height()),
      m_style(page->book()->style())
{
}

static BookBlock::Ptr fetchBlock(int index, const QList<BookBlockFactory::Ptr> &factories, const ItemId &id, QList<BookBlock::Ptr> &cache)
{
    BookBlock::Ptr block = factories[index]->item(id);
    cache << block;
    return block;
}

static BookTextPosition nextLinePosition(const QList<BookBlockFactory::Ptr> &factories,
                                    QList<BookBlock::Ptr> &cache,
                                    const ItemId &id, int body, int block, int line)
{
    BookBlock::Ptr currentBlock = fetchBlock(block, factories, id, cache);
    if (line + 1 < currentBlock->linesCount())
        return createPosition(body, block, currentBlock->lineInfo(line + 1).start);

    while (block + 1 < factories.size()) {
        ++block;
        currentBlock = fetchBlock(block, factories, id, cache);

        if (currentBlock->linesCount() == 0)
            continue;
        return createPosition(body, block, currentBlock->lineInfo(0).start);
    }

    return invalidPosition();
}

BookTextPosition BookPageIterator::nextPage(QList<BookBlock::Ptr> &cache) const
{
    if (!m_position)
        return invalidPosition();

    qreal heightDelta = m_size.height();
    const ItemId id(m_size, m_style);

    for (int i = m_position.block; i < m_blocks.size(); ++i) {
        const BookBlock::Ptr &block = fetchBlock(i, m_blocks, id, cache);

        int lineNumber = (m_position.block == i ? block->lineForPosition(m_position.position) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return createPosition(m_position.body, i, info.start);
            heightDelta -= info.height;
        }
    }

    return invalidPosition();
}

BookTextPosition BookPageIterator::previousPage(QList<BookBlock::Ptr> &cache) const
{
    if (!m_position || (m_position.block == 0 && m_position.position == 0))
        return invalidPosition();

    qreal heightDelta = m_size.height();
    const ItemId id(m_size, m_style);

    if (m_position.block >= m_blocks.size() || m_position.block < 0)
        return invalidPosition();

    int startBlock = m_position.block;
    int startLine = fetchBlock(m_position.block, m_blocks, id, cache)->lineForPosition(m_position.position);

    if (startBlock == 0 && startLine == 0)
        return invalidPosition();

    if (startLine == 0) {
        --startBlock;
        BookBlock::Ptr block = fetchBlock(startBlock, m_blocks, id, cache);
        startLine = block->linesCount();
    }

    for (int i = startBlock; i >= 0; --i) {
        BookBlock::Ptr block = fetchBlock(i, m_blocks, id, cache);

        int lineNumber = (startBlock == i ? startLine : block->linesCount()) - 1;
        while (lineNumber < 0) {
            --i;
            BookBlock::Ptr block = fetchBlock(i, m_blocks, id, cache);
            lineNumber = block->linesCount() - 1;
        }
        for (int j = lineNumber; j >= 0; --j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(heightDelta, info.height))
                return nextLinePosition(m_blocks, cache, id, m_position.body, i, j);
            heightDelta -= info.height;
        }
    }

    return createPosition(m_position.body, 0, 0);
}

void BookPageIterator::setPosition(const BookTextPosition &position)
{
    m_position = position;
}

BookTextPosition BookPageIterator::position() const
{
    return m_position;
}

QList<BookPageIterator::Line> BookPageIterator::pageLines(QList<BookBlock::Ptr> &cache) const
{
    QPointF position(0, 0);
    const ItemId id(m_size, m_style);

    QList<Line> result;

    for (int i = m_position.block; i < m_blocks.size(); ++i) {
        const BookBlock::Ptr &block = m_blocks[i]->item(id);
        cache << block;

        int lineNumber = (m_position.block == i ? block->lineForPosition(m_position.position) : 0);
        const int linesCount = block->linesCount();
        for (int j = lineNumber; j < linesCount; ++j) {
            const BookBlock::LineInfo info = block->lineInfo(j);
            if (fuzzyLess(m_size.height() - position.y(), info.height))
                return result;

            result.append({ block, j, position });
            position.ry() += info.height;
        }
    }

    return result;
}
