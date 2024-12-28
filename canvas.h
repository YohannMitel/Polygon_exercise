#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QVector>
#include <QResizeEvent>
#include "mypolygon.h"




class Canvas : public QWidget {
    Q_OBJECT
 public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    /*
     * show/hide the drawing of the triangles
     */
    void showTriangles(bool state) { drawTriangleBorders=state; repaint(); }
    void showClosestLine(bool state) { drawClosestLine=state; repaint(); }
signals:
    /*
     * Update the status bar string with the parameter
     */
    void updateSB(QString s);
private:
    Vector2D mousePos;        // Position de la souris
    Vector2D closestHPoint;   // Point de projection le plus proche
    bool mousePosSet = false;
    QVector<MyPolygon*> polygons; // the array of polygons
    float scale; // scale of the drawing to fill the window
    QPoint origin; // origin of the drawing area
    bool drawTriangleBorders; // show/hide the drawing of the triangles
    bool drawClosestLine =true ; // show/hide the drawing of the triangles
};

#endif // CANVAS_H
