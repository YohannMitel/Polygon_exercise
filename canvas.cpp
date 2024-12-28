#include "canvas.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDebug>

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    MyPolygon *S1 = new MyPolygon(20);
    S1->addVertex(120,40);
    S1->addVertex(400,160);
    S1->addVertex(320,400);
    S1->addVertex(40,80);
    S1->isConvex() ?  S1->setColor(Qt::blue) : S1->setColor(Qt::cyan);
    S1->earClipping();
    qDebug() << "Surface : " << S1->surface();
    polygons.push_back(S1);

    MyPolygon *S2 = new MyPolygon(20);
    S2->addVertex(620,40);
    S2->addVertex(820,60);
    S2->addVertex(900,160);
    S2->addVertex(820,400);
    S2->addVertex(680,240);
    S2->addVertex(540,80);
    S2->isConvex() ?  S2->setColor(Qt::blue) : S2->setColor(Qt::cyan);
    S2->earClipping();
    qDebug() << "Surface : " << S2->surface();
    polygons.push_back(S2);

    MyPolygon *S3 = new MyPolygon(20);
    S3->addVertex(500,500);
    S3->addVertex(800,600);
    S3->addVertex(900,900);
    S3->addVertex(400,900);
    S3->addVertex(680,800);
    S3->addVertex(640,650);
    S3->addVertex(240,680);
    S3->isConvex() ?  S3->setColor(Qt::blue) : S3->setColor(Qt::cyan);
    S3->earClipping();
    qDebug() << "Surface : " << S3->surface();
    polygons.push_back(S3);


    drawTriangleBorders=false;
}

Canvas::~Canvas() {
    for (auto &poly : polygons) {
        delete poly;
    }
    polygons.clear();
}

void Canvas::resizeEvent(QResizeEvent *event) {
    int newWidth = event->size().width();
    int newHeight = event->size().height();
    // search max x and y of the polygons
    float maxx=0, maxy=0;
    for (auto &poly:polygons) {
        int n;
        auto vert=poly->getVertices(n);
        for (int i=0; i<n; i++) {
            if (vert[i].x>maxx) maxx=vert[i].x;
            if (vert[i].y>maxy) maxy=vert[i].y;
        }
    }
    float maxs = maxx>maxy?maxx:maxy;
    if (newWidth>newHeight) {
        scale=newHeight/(maxs+40);
        origin.setY(10);
        origin.setX((newWidth-newHeight)/2);
    } else {
        scale=newWidth/(maxs+40);
        origin.setX(10);
        origin.setY((newHeight-newWidth)/2);
    }
}

void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0,0,width(),height(),whiteBrush);

    QPointF points[]={{0,-2},{80,-2},{80,-10},{100,0},{80,10},{80,2},{0,2}};
    painter.save();
    painter.translate(origin.x(),height()-origin.y());
    painter.scale(scale,-scale);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawPolygon(points,7);
    painter.save();
    painter.setBrush(Qt::green);
    painter.rotate(90);
    painter.drawPolygon(points ,7);
    painter.restore ();

    // drawing of the polygons
    for (auto &poly: polygons) {
        poly->draw(painter,drawTriangleBorders);
    }
    painter.restore();

    // draw points in a standard referential
    QFont font("Arial") ;
    font.setPointSize(18);
    painter.setBrush(Qt::black);
    painter.setFont(font);
    Vector2D v;
    for (auto &poly:polygons) {
        int n;
        auto vert=poly->getVertices(n);
        for (int i=0; i<n; i++) {
            v = (vert[i+1]-vert[i]).normed().ortho();
            painter.drawText(QPointF(origin.x()+vert[i].x*scale-20*v.x,height()-origin.y()-vert[i].y*scale+20*v.y),QString::number(i));
        }
    }


    // DRAW nearest line
    if (drawClosestLine && mousePosSet) {
        // Transform coordinates to be in the window's coordinate system
        QPointF transformedMousePos(origin.x() + mousePos.x * scale, height() - origin.y() - mousePos.y * scale);
        QPointF transformedClosestHPoint(origin.x() + closestHPoint.x * scale, height() - origin.y() - closestHPoint.y * scale);

        // Set a color for the line (e.g., black)
        painter.setPen(QPen(Qt::black, 2));

        // Draw the line
        painter.drawLine(transformedMousePos, transformedClosestHPoint);
    }




}

void Canvas::mouseMoveEvent(QMouseEvent* e) {
    // convert event->pos() to canvas coordinates
    float mouseX = float(e->pos().x() - origin.x()) / scale;
    float mouseY = -float(e->pos().y() - height() + origin.y()) / scale;
    Vector2D pt(mouseX, mouseY);
    emit updateSB(QString("Mouse position= (") + QString::number(mouseX, 'f', 1) + "," + QString::number(mouseY, 'f', 1) + ")");

    // check if point is in the polygon
    double nearestDistance = 10000;
    int closestPolygonIndex = -1;
    int nearestPolygonSegment;

    for (int i = 0; i < polygons.size(); i++) {
        NearestEdgeResult res = polygons[i]->nearestEdge(pt);
        double distance = res.distance;
        if (distance < nearestDistance) {
            nearestDistance = distance;
            closestPolygonIndex = i;
            nearestPolygonSegment = res.nearest;
        }
        polygons[i]->changeColor(pt);
    }

    if (closestPolygonIndex != -1 && nearestPolygonSegment != -1) {
        closestHPoint = polygons[closestPolygonIndex]->closestH(pt, nearestPolygonSegment);  // Stores the closest point H
        mousePos = pt;  // Stores the mouse position
        mousePosSet = true;  // Indicates that the mouse position is set

        //qDebug() << "Closest Polygon Index:" << closestPolygonIndex << "with distance:" << nearestDistance;
    } else {
        mousePosSet = false;  // If no nearby polygon is found, the line is not displayed
    }


    update();  // Redraw widget
}
