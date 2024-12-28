#ifndef MYPOLYGON_H
#define MYPOLYGON_H
#include "vector2d.h"
#include <QPainter>
#include <QDebug>
#include "nearestEdgeResult.h"

class Triangle {
public:
    Vector2D *tabPts[3]; // array of 3 pointer to points
    Triangle() {}
    Triangle(Vector2D *p_p0,Vector2D *p_p1,Vector2D *p_p2) {
        tabPts[0]=p_p0;
        tabPts[1]=p_p1;
        tabPts[2]=p_p2;
    }
    // to complete
    /*bool isOnTheLeft(const Vector2D &P,int iA, int iB) {
        Vector2D AB = *tabPts[iB+1]-*tabPts[iA],
            AP = P-*tabPts[iA];
        return (AB.x*AP.y - AB.y*AP.x)>=0;
    }*/
    bool isOnTheLeft(const Vector2D &P, int iA, int iB) {
        Vector2D AB = *tabPts[iB ] - *tabPts[iA];
        Vector2D AP = P - *tabPts[iA];
        return (AB.x * AP.y - AB.y * AP.x) >= 0;
    }

    bool isInside(const Vector2D &P) {
        return (isOnTheLeft(P,0,1) && isOnTheLeft(P,1,2) && isOnTheLeft(P,2,0));
    }
    double surface(){
        Vector2D AB = *tabPts[1] -* tabPts[0];
        Vector2D AC = *tabPts[2] -* tabPts[1];
        return 0.5*std::abs(AB.x*AC.y-AB.y*AC.x);
    }


};

class MyPolygon {
private:
    int Nmax; // maximum number of vertices for the current Polygon
    int N; // current number of Vertices
    Vector2D *tabPts; // array of vertex positions
    QVector<Triangle> triangles; // array of triangles of the triangulation
    QColor currentColor; // current drawing color

public:

    MyPolygon(int p_Nmax):Nmax(p_Nmax) {
        N=0;
        tabPts = new Vector2D[Nmax+1];
        currentColor=Qt::green;
    }
    ~MyPolygon() {
        delete [] tabPts;
    }



    Vector2D *getVertices(int &n) { n=N; return tabPts; };
    void addVertex(float x,float y);
    void draw(QPainter &painter,bool showTriangles);
    void setColor(const QColor c) { currentColor=c; }

    void changeColor(const Vector2D &pt) {
        //isInside(pt) ? setColor(Qt::red) : setColor(currentColor) ;
        auto it=triangles.begin();
        while (it!=triangles.end() && !(it->isInside(pt))) {
            it++;
        }
        currentColor=(it==triangles.end()?Qt::green:Qt::red);
        //currentColor =  (isInside(pt) ?Qt::red : Qt::red );
    }
    double surface(){
        double s=0;
        for(auto &tri:triangles){
            s+=tri.surface();
        }
        return s;
    }
    bool isOnTheLeft(const Vector2D &P,int i);
    bool isConvex();
    bool isInside(const Vector2D &P);
    void earClipping();

    double distanceToEdge(const Vector2D &M,int i);
    NearestEdgeResult nearestEdge(const Vector2D &M);
    Vector2D closestH(const Vector2D &M,int &nearest);
};

#endif // MYPOLYGON_H
