#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H

#include <QList>
#include <QPoint>

#include "GradientMapMax.h"
#include "PixelGradientInfo.h"

class ConnectedComponent
{
private:

    static int _s_next_id;

private:

    int _id;

    QList<QPoint> _ends;
    QList<QPoint> _points;

public:

    ConnectedComponent();

    const QList<QPoint>& ends() const { return _ends; }

    // À implémenter
    const QList<QLine> segments() const;

public:

    static void fromGradientMapMax(GradientMapMax& map,
                                   QList<ConnectedComponent>& components);

public: // ToDo JRA: ConnectedComponent: section publique à privatiser

    // À implémenter
    const QList<QList<QPoint> > subComponents() const;

    static void getConnectedComponent(GradientMapMax& map,
                                      const QPoint& point,
                                      const QPoint& previous,
                                      ConnectedComponent& component);

    static void getNeighbours(GradientMapMax& map,
                              const QPoint& point,
                              QList<QPoint>& neighbours);
};

#endif // CONNECTEDCOMPONENT_H
