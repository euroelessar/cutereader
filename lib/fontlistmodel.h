#ifndef FONTLISTMODEL_H
#define FONTLISTMODEL_H

#include <QStringListModel>
#include <QFontDatabase>

namespace CuteReader {

class FontListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit FontListModel(QObject *parent = 0);

    Q_INVOKABLE QVariantList standardSizes() const;
    Q_INVOKABLE QStringList families() const;

private:
    QFontDatabase m_db;
};

} //namespace CuteReader

#endif // FONTLISTMODEL_H
