#include "modelmath.h"
#include <QList>
#include <QLine>

ModelMath::ModelMath()
{
}

QList<QList<QLine> > ModelMath::modelMath() const
{
    return _modelMath;
}

void ModelMath::getSegments(GradientMapMax &map)
{
    for(int row = 0; row < map.height(); row++)
    {
        for(int col = 0; col < map.width(); col++)
        {
            map.composantAt(row, col).visite = false;
        }
    }

    for(int row = 1; row < map.height()-1; row++)
    {
        for(int col = 1; col < map.width()-1; col++)
        {
            QPoint P_courant (col, row);

            if((map.composantAt(P_courant).valS == 255) && (map.composantAt(P_courant).visite == false))
            {
                QList<QLine> composant;
                getSegments(map, P_courant, composant);
                _modelMath += composant;
            }
        }
    }

}

void ModelMath::getSegments(GradientMapMax &map, QPoint Sdebut, QList<QLine> &l, QPoint d)
{
    QPoint Sfin = Sdebut;
    QList<QPoint> intersections;

    while(map.composantAt(Sfin).valS == 255)
    {
        if(otherNeighbour(map, Sfin, d) == true)//Sfin a un voisin dans une direction != d)
        {
            intersections += Sfin;
        }

        map.composantAt(Sfin).visite = true;
        Sfin += d;

        if(map.composantAt(Sfin).visite == true)//valid
            break;
    }

    if ((d.isNull()==false) || (intersections.isEmpty() == true))
    {
        if(map.composantAt(Sfin).valS == 255)
        {
            QLine segment(Sdebut, Sfin);
            l += segment;
        }
        else
        {
            QLine segment(Sdebut, (Sfin - d));
            l += segment;
        }
    }

    for(int i = 0; i < intersections.size(); i++)
    {
        QList<QPoint> voisins = neighbours(map, intersections[i]);
        for(int j = 0; j < voisins.size(); j++)
        {
            if(map.composantAt(voisins[j]).visite == false)
                getSegments(map, intersections[i], l, (voisins[j] - intersections[i]));
        }
    }
}

QList<QPoint> ModelMath::neighbours(GradientMapMax &map, QPoint pixel)
{
    QList<QPoint> voisins;

    for(int row = pixel.y() - 1; row <= pixel.y() + 1; row++)
    {
        for(int col = pixel.x() - 1; col <= pixel.x()+1; col++)
        {
            if((row == pixel.y()) && (col == pixel.x()))
                continue;

            if(map.composantAt(row, col).valS == 255)
            {
                QPoint temp (col, row);
                voisins += temp;
            }
        }
    }

    return voisins;
}

bool ModelMath::otherNeighbour(GradientMapMax &map, QPoint pixel, QPoint dir)
{
    QPoint temp, temp2;
    temp = pixel+dir;
    temp2 = pixel-dir;

    for(int row = pixel.y() - 1; row <= pixel.y() + 1; row++)
    {
        for(int col = pixel.x() - 1; col <= pixel.x()+1; col++)
        {
            if((row == pixel.y()) && (col == pixel.x()))
                continue;

            if((row == temp.y()) && (col == temp.x()))
                continue;
            if((row == temp2.y()) && (col == temp2.x()))
                continue;

            if(map.composantAt(row, col).valS == 255)
            {
                return true;
            }
        }
    }

    return false;
}



/*
void getSegments(Point Sdébut, Vecteur d, Liste<Segment> l)
{
    Point Sfin = Sdébut;
    Liste<Point> intersections;

    tant que (Sfin valide)
    {
        si (Sfin a un voisin dans une direction != d)	// direction non-orientée
                intersections += Sfin;

        Sfin.visité = true;
        Sfin += d;

        si (Sfin visité)
                break;
    }

    si (d != 0 ou intersections vide)
    {
        si (Sfin valide)
                l += segment(Sdébut, Sfin);
        sinon
                l += segment(Sdébut, Sfin - d);
    }

    pour (chaque intersection i)
            pour (chaque voisin v de i)
            si (v pas visité)
            getSegments(i, v-i, l);
}
*/
