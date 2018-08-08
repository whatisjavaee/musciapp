#include "filelineplayer.h"

FileLinePlayer::FileLinePlayer()
{
    setFlag(ItemHasContents, true);
}
QSGNode* FileLinePlayer::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*)
{
    QSGGeometryNode* node = nullptr;
    QSGGeometry* geometry = nullptr;
    if (!oldNode)
    {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 1000);
        geometry->setLineWidth(1);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor(QColor(0, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else
    {
        node = static_cast<QSGGeometryNode*>(oldNode);
        geometry = node->geometry();
        geometry->allocate(1000);
    }
    QSGGeometry::Point2D* vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < 1000; i++)
    {
        vertices[i].set(qrand() % int(this->width()), qrand() % int(this->height()));
        vertices[i].set(qrand() % (int)this->width(), qrand() % (int)this->height());
    }
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}
