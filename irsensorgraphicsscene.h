#ifndef IRSENSORGRAPHICSSCENE_H
#define IRSENSORGRAPHICSSCENE_H
#include <QGraphicsScene>
#include <QPainter>
#include <QObject>

class IRSensorGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit IRSensorGraphicsScene(QObject* parent=0);
    void drawDistanceSensors(QList<double> distances);
private:
    QBrush* brush;
    QList<QPoint> sensorCenters;

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

};

#endif // IRSENSORGRAPHICSSCENE_H
