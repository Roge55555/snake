#include <QPainter>

#include "constants.h"
#include "food.h"


static const qreal FOOD_RADIUS = 3.0;

Food::Food(qreal x, qreal y)
{

    setPos(x, y);
    setData(GD_Type, GO_Food);
}

QRectF Food::boundingRect() const
{
    return QRectF(-TILE_SIZE,    -TILE_SIZE,
                   TILE_SIZE * 2, TILE_SIZE * 2 );
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);


    if(tipe == 1)painter->fillPath(shape(), Qt::black);
    if(tipe == 2)painter->fillPath(shape(), Qt::white);
    if(tipe == 3)painter->fillPath(shape(), Qt::cyan);



    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(TILE_SIZE / 2, TILE_SIZE / 2), FOOD_RADIUS, FOOD_RADIUS);
    return p;
}
