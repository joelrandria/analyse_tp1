#include "ConnectedComponent.h"

int ConnectedComponent::_s_next_id = 0;

ConnectedComponent::ConnectedComponent()
    :_id(_s_next_id++)
{
}

void ConnectedComponent::fromGradientMapMax(GradientMapMax &map, QList<ConnectedComponent> &components)
{
    PixelGradientInfo pixel;

    int map_width = map.width();
    int map_height = map.height();

    // Réinitialisation de l'ensemble de composantes & des infos attachées aux pixels spécifiques à la connexité
    components.clear();
    map.resetConnectedComponentsInfo();

    for (int row = 1; row < (map_height - 1); ++row)
    {
        for (int col = 1; col < (map_width - 1); ++col)
        {
            pixel = map.composantAt(row, col);
            if (pixel.isAccepted() && !pixel.isInConnectedComponent())
            {
                // Point de départ de la nouvelle composante connexe
                QPoint start(col, row);

                // La nouvelle composante connexe
                ConnectedComponent component;

                // Obtention des voisins du point de départ
                QList<QPoint> startNeighbours;
                getNeighbours(map, start, startNeighbours);

                if (startNeighbours.size() <= 1)
                {
                    // Le point de départ est une extrémité => Insertion dans les extrémités de la composante
                    component._ends += start;
                    map.composantAt(start.y(), start.x()).end = true;
                }

                // Obtention de l'intégralité de la composante
                getConnectedComponent(map, start, QPoint(), component);

                // Ajout de la composante à l'ensemble finale de composante
                components += component;
            }
        }
    }
}

void ConnectedComponent::getConnectedComponent(GradientMapMax &map,
                                               const QPoint &point,
                                               const QPoint &previous,
                                               ConnectedComponent& component)
{
    QList<QPoint> pointNeighbours;

    map.composantAt(point.y(), point.x()).connectedComponentId = component._id;
    component._points += point;

    // Obtention des voisins du point
    getNeighbours(map, point, pointNeighbours);

    if ((pointNeighbours.size() == 1) && (pointNeighbours.first() == previous))
    {
        component._ends += point;
        map.composantAt(point.y(), point.x()).end = true;
    }
    else
    {
        for (int i = 0; i < pointNeighbours.size(); ++i)
        {
            QPoint neighbour = pointNeighbours[i];
            if (neighbour != previous)
            {
                PixelGradientInfo neighbourInfo = map.composantAt(neighbour.y(), neighbour.x());

                if (!neighbourInfo.isInConnectedComponent())
                {
                    // Le voisin n'appartient à aucune composante => Récursion
                    getConnectedComponent(map, neighbour, point, component);
                }
                else if (neighbourInfo.connectedComponentId == component._id)
                {
                    // Cycle => On s'assure que le voisin ne figure plus dans les extrémités de la composante
                    component._ends.removeAll(neighbour);
                    map.composantAt(neighbour.y(), neighbour.x()).end = false;
                }
            }
        }
    }
}

void ConnectedComponent::getNeighbours(GradientMapMax &map,
                                       const QPoint &point,
                                       QList<QPoint> &neighbours)
{
    neighbours.clear();

    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            if (i == 0 && j == 0)
                continue;

            if ((point.y() + i < 0) || (point.y() + i >= map.height())
                    || (point.x() + j < 0) || (point.x() + j >= map.width()))
                continue;

            if (map.composantAt(point.y() + i, point.x() + j).isAccepted())
                neighbours += QPoint(point.x() + j, point.y() + i);
        }
    }
}
