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

    components.clear();//tts les ensembles connexe

    for (int row = 1; row < (map_height - 1); ++row)
    {
        for (int col = 1; col < (map_width - 1); ++col)
        {
            pixel = map.composantAt(row, col);
            if (pixel.isAccepted() && !pixel.isInConnectedComponent())
            {
                QPoint start(col, row);//point de début pour trouve notre ens connexe

                ConnectedComponent component;//notre nouvelle ens connexe

                QList<QPoint> startNeighbours;//les des voisin du 1er point
                getNeighbours(map, start, startNeighbours);//trouver tout les voisin du 1er pt

                if (startNeighbours.size() <= 1) // ajouter le 1er point dans le cas ou il est extraimeter
                   {
                    component._ends += start;
                    map.composantAt(start.y(), start.x()).end = true;
                    }
                getConnectedComponent(map, start, QPoint(), component);//crée un nouveau l'ensemble connexe

                components += component;//ajout de nouveau ens connexe avec les autre ensemble
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

    getNeighbours(map, point, pointNeighbours);//ajout dans pointNeigbord tt les voisin du point
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

                if (!neighbourInfo.isInConnectedComponent())//si il n'est pas deja dans un ens connexe
                    getConnectedComponent(map, neighbour, point, component);
                else if (neighbourInfo.connectedComponentId == component._id)//teste cycle
                {
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

            if (map.composantAt(point.y() + i, point.x() + j).isAccepted())
                neighbours += QPoint(point.x() + j, point.y() + i);
        }
    }
}
