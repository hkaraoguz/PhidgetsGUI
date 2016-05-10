#include "irsensorgraphicsscene.h"
#include <math.h>
#include <QDebug>

IRSensorGraphicsScene::IRSensorGraphicsScene(QObject *parent):QGraphicsScene(parent)
{

    brush = new QBrush(Qt::black,Qt::SolidPattern);


}
void IRSensorGraphicsScene::drawDistanceSensors(QList<double> distances)
{
    int angleoffset = 360/distances.size();

    QPoint sensorCenter;

    sensorCenters.clear();

    int j= 90;

    for(int i = 0; i < distances.size(); i++)
    {
        //for(int j = 90; j >=-angleoffset*(distances.size()-1); j -= angleoffset)
        //{
            double x = 5*distances[i]*cos((double)j*M_PI/180);
            double y = -5*distances[i]*sin((double)j*M_PI/180);
            sensorCenter.setX(x);
            sensorCenter.setY(y);
            sensorCenters.append(sensorCenter);
            //qDebug()<<j<<sensorCenter.x()<<sensorCenter.y()<<distances[i];

            j -= angleoffset;
            //sensorCenter.setX();

      //  }


    }



}

void IRSensorGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{



  // brush.setColor(Qt::black);
   //brush.setStyle(Qt::SolidPattern);

   this->setBackgroundBrush(*brush);

    painter->setPen(Qt::red);

    painter->fillRect(rect, this->backgroundBrush());

    QPoint cnt = rect.center().toPoint();
    painter->drawEllipse(cnt,25,25);
    painter->drawEllipse(cnt,50,50);
    painter->drawEllipse(cnt,100,100);
    painter->drawEllipse(cnt,150,150);
    //painter->drawEllipse(cnt,400,400);


    painter->setPen(Qt::blue);
    QPoint point;
    QPoint cent = rect.center().toPoint();

    point.setY(cent.y()-30);
    point.setX(cent.x()-15);
    painter->drawText(point,"5 cm");

    point.setY(cent.y()-55);
    point.setX(cent.x()-20);
    painter->drawText(point,"10 cm");

    point.setY(cent.y()-105);
    point.setX(cent.x()-20);
    painter->drawText(point,"20 cm");

    point.setY(cent.y()-155);
    point.setX(cent.x()-20);
    painter->drawText(point,"30 cm");

   // painter->setPen(Qt::green);
    QPen linepen(Qt::yellow);
    linepen.setCapStyle(Qt::RoundCap);
    linepen.setWidth(5);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(linepen);
    painter->drawPoint(0,0);
    linepen.setColor(Qt::green);
    painter->setPen(linepen);



   // painter->drawLine(0,1,25,1);

    for(int i = 0; i < sensorCenters.size(); i++)
    {
        painter->drawPoint(sensorCenters[i]);
    }

}
