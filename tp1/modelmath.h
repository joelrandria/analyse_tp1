#ifndef MODELMATH_H
#define MODELMATH_H
#include <QPoint>
#include <QVector2D>
#include <QList>
#include <QLine>
#include "GradientMapMax.h"

class ModelMath
{
private:

    QList<QList<QLine> > _modelMath;

public:

    ModelMath();
    void getSegments(GradientMapMax &map);

    QList<QList<QLine> > modelMath() const;

private:

    static void getSegments(GradientMapMax &map, QPoint Sdebut, QList<QLine> &l, QPoint d = QPoint());
    static QList<QPoint> neighbours(GradientMapMax &map, QPoint pixel);
    static bool otherNeighbour(GradientMapMax &map, QPoint pixel, QPoint dir);

};

#endif // MODELMATH_H
