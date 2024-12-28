#include "mypolygon.h"
#include <QDebug>
#include <QString>
#include "nearestEdgeResult.h"
void MyPolygon::addVertex(float x, float y) {
    assert(N<Nmax);
    tabPts[N].x=x;
    tabPts[N].y=y;
    N++;
    tabPts[N].x=tabPts[0].x;
    tabPts[N].y=tabPts[0].y;

}

void MyPolygon::draw(QPainter &painter,bool showTriangles) {
    QPen pen(Qt::black);
    pen.setWidth(3);

    QPoint *points=new QPoint[N];
    for (int i=0; i<N; i++) {
        points[i].setX(tabPts[i].x);
        points[i].setY(tabPts[i].y);
    }
    painter.setBrush(currentColor);
    painter.setPen(pen);
    painter.drawPolygon(points,N,Qt::OddEvenFill);

    if (showTriangles) {
        painter.setPen(Qt::blue);
    } else {
        painter.setPen(Qt::NoPen);
    }
    for (auto &t:triangles) {

        painter.drawLine(t.tabPts[0]->x,t.tabPts[0]->y,t.tabPts[1]->x,t.tabPts[1]->y);
        painter.drawLine(t.tabPts[1]->x,t.tabPts[1]->y,t.tabPts[2]->x,t.tabPts[2]->y);
        painter.drawLine(t.tabPts[2]->x,t.tabPts[2]->y,t.tabPts[0]->x,t.tabPts[0]->y);
    }
    delete [] points;
}

bool MyPolygon::isOnTheLeft(const Vector2D &P,int i) {
    Vector2D AB = tabPts[i+1]-tabPts[i],
        AP = P-tabPts[i];
    return (AB.x*AP.y - AB.y*AP.x)>=0;
}
bool MyPolygon::isConvex() {
    int i=0;
    while (i<N && isOnTheLeft(tabPts[(i+2)%N],i)) {
        i++;
    }
    return (i==N);
}

bool MyPolygon::isInside(const Vector2D &P) {
    int i=0;
    while (i<N && isOnTheLeft(P,i)) {
        i++;
    }
    return (i==N);
}
void MyPolygon::earClipping(){
    triangles.clear();
    QVector<Vector2D*> tmp;
    for(int i = 0; i<N; i++){
        tmp.push_back(&tabPts[i]);
    }
    tmp.push_back(&tabPts[0]);
    tmp.push_back(&tabPts[1]);

    int i=0;
    do {
        Triangle T(tmp[i],tmp[i+1],tmp[i+2]);
        int j = i+3;
        while(j<tmp.size()-2 && !T.isInside(*tmp[j])){
            j++;
        }
        if(j==tmp.size()-2){
            triangles.push_back(T);
            tmp.removeAt(i+1);
            i = 0;
        }else{
            i++;
        }
    }while (tmp.size()>=5);

}


NearestEdgeResult MyPolygon::nearestEdge(const Vector2D &M){
    double distance = 1000;
    int nearest = -1;


    for(int i = 0 ; i < N ; i++){
        double d = distanceToEdge(M, i );
        //qDebug() << QString::number(N) << distanceToEdge(M, i );
        if (distance > d){
            distance =  d;
            nearest = i;
        }
    }
    return NearestEdgeResult{distance,nearest};
}



double MyPolygon::distanceToEdge(const Vector2D &M, int i) {
    Vector2D AB = tabPts[i+1] - tabPts[i];  // Vector AB representing the edge
    Vector2D AM = M - tabPts[i];  // Vector AM from the edge start to point M
    double lAB = AB.length();  // Length of edge AB
    double AH = (AB.x * AM.x + AB.y * AM.y) / lAB;  // Projection of AM onto AB

    if (AH < 0) {  // The perpendicular projection falls before the start of the edge
        return AM.length();
    } else if (AH > lAB) {  // The perpendicular projection falls beyond the end of the edge
        return (M - tabPts[i+1]).length();
    } else {  // The perpendicular projection falls within the edge
        return abs(AB.x * AM.y - AB.y * AM.x) / lAB;
    }
}

Vector2D MyPolygon::closestH(const Vector2D &M,int &nearest){

    return Vector2D::projection(tabPts[nearest+1],  tabPts[nearest], M);
}
