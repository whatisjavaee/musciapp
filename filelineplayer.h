#ifndef FILELINEPLAYER_H
#define FILELINEPLAYER_H

#include <QQuickItem>
#include<QSGNode>
#include<QSGFlatColorMaterial>
#include<QSGRectangleNode>
#include"blockqsgrectanglenode.h"
class FileLinePlayer : public QQuickItem
{
    Q_OBJECT
public:
    FileLinePlayer();
protected:
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);

};

#endif // FILELINEPLAYER_H
