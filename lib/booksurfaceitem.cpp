#include "booksurfaceitem.h"

#include <QtGui>
#include <QtConcurrent>
#include <functional>

static void reduceHeights(qreal &result, qreal height)
{
    result += height;
//    qDebug() << result;
}

BookSurfaceItem::BookSurfaceItem(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true);
    
    QElapsedTimer timer;
    timer.start();
    
//    m_blocks = parse_fb2(QStringLiteral("/home/elessar/.books/Oldi_Oykumena_1_Kukolnik.131765.fb2"));
//    qDebug() << m_blocks.size();
    
    qDebug() << "Book parse: " << timer.elapsed() << " ms";
    
    connect(this, &BookSurfaceItem::widthChanged, [this] {
        const qreal width = this->width();
        
        std::function<qreal (const BookTextBlock::Ptr &)> map
                = [width] (const BookTextBlock::Ptr &block) {
            block->setWidth(width);
            return block->height();
        };
        
////        std::function<void (qreal &, qreal)> reduce
//        auto reduce = [] (qreal &result, qreal height) {
//            result += height;
////            qDebug() << result;
//        };
        
        QtConcurrent::mappedReduced(m_blocks, map, reduceHeights);
        
        /*, [] (qreal &result, qreal height) {
            result += height;
//            qDebug() << result;
        });*/
//        QElapsedTimer timer;
//        timer.start();
//        int lines = 0;
//        for (auto &block : m_blocks) {
//            block->setWidth(width());
//            lines += block->linesCount();
//        }
        
//        qDebug() << "width: " << width() << ", lines: " << lines << ", time: " << timer.elapsed() << " ms";
    });
}

BookSurfaceItem::~BookSurfaceItem()
{
}

void BookSurfaceItem::paint(QPainter *painter)
{
    qDebug() << painter->viewport();
    
    QPointF position(0, 0);
    QElapsedTimer timer;
    timer.start();
    
    qreal currentHeight = 0;
    
    for (const BookTextBlock::Ptr &block : m_blocks) {
//        qDebug() << position;
        block->setWidth(width());
        block->draw(painter, position);
        position.setY(position.y() + block->height());
        
        currentHeight += block->height();
        
//        break;
        
        if (currentHeight > height())
            break;
    }
    
    qDebug() << "height calc: " << timer.elapsed() << " ms";
}

QList<BookTextBlock::Ptr> BookSurfaceItem::blocks() const
{
    return m_blocks;
}
