#include "GradientMapMax.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <math.h>

using namespace std;

GradientMapMax::GradientMapMax(const Image &image, const GradientKernel &kernel, const bool normaliser) : GradientMap(image,kernel)
{
    float normalizationFactor = kernel.getNormalizationFactor();
    int maxValueIndex;
    float MaxValue;

    resize();

    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            Gradient gradient = getGradientAt(row, col);

            maxValueIndex = gradient.getMaxValueDirection();
            MaxValue = abs(gradient.getValueAt(maxValueIndex));

            if(normaliser == true) MaxValue *= normalizationFactor;

            _maxGradients[row][col].val = MaxValue;
            _maxGradients[row][col].dir = maxValueIndex;
        }
    }
}

void GradientMapMax::seuillageHyest(float seuilH, float seuilBas)
{
    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            if(_maxGradients[row][col].val < seuilH)//ou egale
                _maxGradients[row][col].valS = 0;
            else _maxGradients[row][col].valS = 255;
        }
    }

    seuillageBas(seuilBas);
}

void GradientMapMax::affinage()
{
    affinageX();
    affinageY();
    affinageYX();
    affinageX_Y();
    affinageV4();
}


PixelGradientInfo& GradientMapMax::composantAt(int row, int col)
{
    return _maxGradients[row][col];
}

PixelGradientInfo& GradientMapMax::composantAt(QPoint point)
{
    return _maxGradients[point.y()][point.x()];
}

void GradientMapMax::Bresenham(int row1,int col1,int row2,int col2)
{
    int drow, dcol, i, e;
    int incR, incC, inc1, inc2;
    int row,col;

    drow = abs(row2 - row1);
    dcol = abs(col2 - col1);

    incR = 1;
    if(row2 < row1) incR = -1;
    incC = 1;
    if(col2 < col1) incC = -1;
    row=row1;
    col=col1;

    _maxGradients[row][col].valS = 255;

    if(drow > dcol)
    {
//        _maxGradients[row][col].valS = 255;
        e = 2*dcol - drow;
        inc1 = 2*( dcol -drow);
        inc2 = 2*dcol;
        for(i = 0; i < drow; i++)
        {
            if(e >= 0)
            {
                col += incC;
                e += inc1;
            }
            else e += inc2;
            row += incR;
            _maxGradients[row][col].valS = 255;
        }
    }
    else
    {
//        _maxGradients[row][col].valS = 255;
        e = 2*drow - dcol;
        inc1 = 2*( drow - dcol);
        inc2 = 2*drow;
        for(i = 0; i < dcol; i++)
        {
            if(e >= 0)
            {
                row += incR;
                e += inc1;
            }
            else e += inc2;
            col += incC;
            //draw_pixel(x,y, BLACK);
            _maxGradients[row][col].valS = 255;
        }
    }
}

void GradientMapMax::resetConnectedComponentsInfo()
{
    int _width = width();
    int _height = height();

    for (int row = 0; row < _height; ++row)
        for (int col = 0; col < _width; ++col)
            composantAt(row, col).resetConnectedComponentInfo();
}

void GradientMapMax::fermetureDirectionContour()
{
    for (int row = 0; row < _rows; ++row)
    {
        for (int col = 0; col < _cols; ++col)
        {
            PixelGradientInfo pix = composantAt(row, col);
            if (pix.end)
            {
                QPoint source(col, row);

                QVector2D edgeLocalDirection = getEndpointLocalDirection(source, 5);

                if (!edgeLocalDirection.isNull())
                {
                    QPoint target = findMatchingEndpoint(source, edgeLocalDirection, cos(M_PI / 4), 15);
                    if (!target.isNull())
                    {
                        Bresenham(source.y(), source.x(), target.y(), target.x());

                        composantAt(source.y(), source.x()).end = false;
                        composantAt(target.y(), target.x()).end = false;
                    }
                }
            }
        }
    }
}
void GradientMapMax::getEndpointConnectedPoints(const QPoint& point, int maxAnteriorityDepth, QList<QPoint>& points)
{
    if (maxAnteriorityDepth <= 0)
        return;

    points += point;

    PixelGradientInfo refPixel = composantAt(point.y(), point.x());

    for (int drow = -1; drow <= 1; ++drow)
    {
        for (int dcol = -1; dcol <= 1; ++dcol)
        {
            if (drow == 0 && dcol == 0)
                continue;

            QPoint framePoint(point.x() + dcol, point.y() + drow);
            if (framePoint.x() < 0 || framePoint.x() >= _cols || framePoint.y() < 0 || framePoint.y() >= _rows)
                continue;

            PixelGradientInfo testPixel = composantAt(framePoint.y(), framePoint.x());
            if (testPixel.connectedComponentId == refPixel.connectedComponentId && !points.contains(framePoint))
                getEndpointConnectedPoints(framePoint, maxAnteriorityDepth - 1, points);
        }
    }
}
QVector2D GradientMapMax::getEndpointLocalDirection(const QPoint& point, int maxAnteriorityDepth)
{
    QList<QPoint> connectedPoints;

    getEndpointConnectedPoints(point, maxAnteriorityDepth, connectedPoints);

    if (connectedPoints.size() < 2)
        return QVector2D();

    QPoint sumVector;
    for (int i = 0; i < connectedPoints.size() - 1; ++i)
        sumVector += connectedPoints[i] - connectedPoints[i + 1];

    return QVector2D(sumVector / (connectedPoints.size() - 1));
}

QPoint GradientMapMax::findMatchingEndpoint(const QPoint& source, QVector2D sourceDirection, float toleranceCosAngle, int searchFrameSize)
{
    int searchFrameHalfSize = searchFrameSize / 2;

    sourceDirection.normalize();

    PixelGradientInfo sourcePixel = composantAt(source.y(), source.x());

    for (int drow = -searchFrameHalfSize; drow <= searchFrameHalfSize; ++drow)
    {
        for (int dcol = -searchFrameHalfSize; dcol <= searchFrameHalfSize; ++dcol)
        {
            if (drow == 0 && dcol == 0)
                continue;

            QPoint testPoint(source.x() + dcol, source.y() + drow);
            if (testPoint.x() < 0 || testPoint.x() >= _cols || testPoint.y() < 0 || testPoint.y() >= _rows)
                continue;

            PixelGradientInfo testPixel = composantAt(testPoint.y(), testPoint.x());
            if (testPixel.end == true && testPixel.connectedComponentId != sourcePixel.connectedComponentId)
            {
                QVector2D targetDirection(testPoint - source);
                targetDirection.normalize();

                float targetCosAngle = QVector2D::dotProduct(sourceDirection, targetDirection);
                if (targetCosAngle > toleranceCosAngle)
                    return testPoint;
            }
        }
    }

    return QPoint();
}

void GradientMapMax::fermetureDirectionGradient()
{
    Pixel p1,p2;

    for(int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++)
        {
            if(_maxGradients[row][col].end == true)

            {
                p1.r = row;
                p1.c = col;

                Pixel prev = neigbordPos(p1);//pos(-1,-1) si il n'a pas de voisins

                p2 = recherchExtr(p1, _maxGradients[row][col].dir, prev);

                if (p2.r != -1)
                {
//                    qDebug() << QString("<%1, %2> => <%3, %4> (dir = <%3, %4>)")
//                                .arg(p1.r).arg(p1.c)
//                                .arg(p2.r).arg(p2.c);

                    //_maxGradients[p1.r][p1.c].end = false;
                    //_maxGradients[p2.r][p2.c].end = false;
                    Bresenham(p1.r,p1.c,p2.r,p2.c);
                }
                else
                {
//                    qDebug() << QString("Aucun raccordement trouvÃ© pour <%1, %2>").arg(p1.r).arg(p1.c);
                }
            }

        }
    }
}

Pixel GradientMapMax::neigbordPos(Pixel pix)
{
    Pixel position;
    position.r = -1;
    position.c = -1;

    for(int row = pix.r-1; row <= pix.r+1; row++)
    {
        for(int col = pix.c-1; col <= pix.c+1; col++)
        {
            if (row == pix.r && col == pix.c)
                continue;

            if (_maxGradients[row][col].valS != 0)
            {
                position.r = row;
                position.c = col;
                return position;
            }
        }
    }

   return position;
}
Pixel GradientMapMax::recherchExtr(Pixel origine, int direction, Pixel preced)
{
//    qDebug() << QString("recherchExtr(origine=<%1,%2>, preced=<%3,%4>)")
//                .arg(origine.r).arg(origine.c)
//                .arg(preced.r).arg(preced.c);
    Pixel temp;
    temp.r = -1;
    temp.c = -1;

    if(preced.r == -1)
    {
        Pixel virtuel;
        switch (direction)
        {
        case 0:
            virtuel.r = origine.r -1;
            virtuel.c = origine.c;
            recherchExtr(origine, direction, virtuel);

            virtuel.r = origine.r +1;
            recherchExtr(origine, direction, virtuel);
            break;

        case 1:
            virtuel.c = origine.c +1;
            virtuel.r = origine.r;
            recherchExtr(origine, direction, virtuel);

            virtuel.c = origine.c -1;
            recherchExtr(origine, direction, virtuel);
            break;

        case 2:
            virtuel.r = origine.r +1;
            virtuel.c = origine.c +1;
            recherchExtr(origine, direction, virtuel);

            virtuel.r = origine.r -1;
            virtuel.c = origine.c -1;
            recherchExtr(origine, direction, virtuel);
            break;

        case 3:
            virtuel.r = origine.r -1;
            virtuel.c = origine.c +1;
            recherchExtr(origine, direction, virtuel);

            virtuel.r = origine.r +1;
            virtuel.c = origine.c -1;
            recherchExtr(origine, direction, virtuel);
            break;
        }
    }
    else
    {
        switch (direction)
        {
        case 0:
            if(preced.r == origine.r)  return temp;
            preced.c = origine.c;
            break;

        case 1:
            if(preced.c == origine.c) return temp;
            preced.r = origine.r;
            break;

        case 2:
            if(((preced.r == origine.r-1)&&(preced.c == origine.c+1))
                    ||((preced.r == origine.r+1)&&(preced.c == origine.c-1)))
                return temp;
            if((preced.c < origine.c)&& (preced.r == origine.r)) preced.r -= 1;
            else if ((preced.c > origine.c)&& (preced.r == origine.r)) preced.r +=1;
            else if ((preced.r < origine.r)&& (preced.c == origine.c)) preced.c -=1;
            else if ((preced.r > origine.r)&& (preced.c == origine.c)) preced.c +=1;
            break;

        case 3:
            if(((preced.r == origine.r-1)&&(preced.c == origine.c-1))
                    ||((preced.r == origine.r+1)&&(preced.c == origine.c+1)))
                return temp;
            if((preced.c < origine.c)&& (preced.r == origine.r)) preced.r += 1;
            else if ((preced.c > origine.c)&& (preced.r == origine.r)) preced.r -=1;
            else if ((preced.r < origine.r)&& (preced.c == origine.c)) preced.c +=1;
            else if ((preced.r > origine.r)&& (preced.c == origine.c)) preced.c -=1;

            break;
        }
    }

    return propagation(preced, origine);
}

Pixel GradientMapMax::propagation(Pixel precedent, Pixel origine)
{
    Pixel dir, Si;

    dir.r = origine.r - precedent.r;
    dir.c = origine.c - precedent.c;

//    qDebug() << QString("dir=<%1, %2>").arg(dir.r).arg(dir.c);
    int maxR = origine.r+15;
    int maxC = origine.c+15;
    int minR = origine.r-15;
    int minC = origine.c-15;
    while (((origine.r < maxR) && (origine.c < maxC) && (origine.r > minR) && (origine.c > minC))
        &&((origine.r < _rows-1) && (origine.c < _cols-1) && (origine.r > 0) && (origine.c > 0)))
    {
       for (int row = origine.r-1; row <= origine.r+1; row++)
        {
            for (int col = origine.c-1; col <= origine.c+1; col++)
            {
                if (row == origine.r && col == origine.c)
                    continue;

                Si.r = row;
                Si.c = col;

                if (prodScal(Si, origine, dir) > 0)//angle Pi
                {
                    if (_maxGradients[Si.r][Si.c].valS != 0)
                    {
                        if (_maxGradients[Si.r][Si.c].end == true)
                        {
                            return Si;
                        }
                        else
                        {
                            Si.r = -1;
                            Si.c = -1;
                            return Si;
                        }
                    }
                }
            }
        }

       origine.r = origine.r + dir.r;
       origine.c = origine.c + dir.c;
    }

    Si.r = -1;
    Si.c = -1;

    return Si;
}

double GradientMapMax::prodScal(Pixel si, Pixel ori, Pixel dir)
{
    double res;
    Pixel temp;

    temp.r = si.r - ori.r;
    temp.c = si.c - ori.c;

    res = (temp.r * dir.r) + (temp.c * dir.c);

    return res;
}

/***********PRIVATE**************/

void GradientMapMax::affinageX()
{
    for (int row = 0; row < _rows; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 0))
            {
                if((_maxGradients[row][col].val < _maxGradients[row][col-1].val) || (_maxGradients[row][col].val < _maxGradients[row][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageY()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 0; col < _cols; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 1))
            {
                if((_maxGradients[row][col].val < _maxGradients[row-1][col].val)||(_maxGradients[row][col].val < _maxGradients[row+1][col].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageYX()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 2))
            {
                if((_maxGradients[row][col].val < _maxGradients[row+1][col-1].val)||(_maxGradients[row][col].val < _maxGradients[row-1][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageX_Y()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if ((_maxGradients[row][col].valS == 255) && (_maxGradients[row][col].dir == 3))
            {
                if((_maxGradients[row][col].val < _maxGradients[row-1][col-1].val)||(_maxGradients[row][col].val < _maxGradients[row+1][col+1].val))
                {
                    _maxGradients[row][col].valS = 0;
                }
            }
        }
    }
}

void GradientMapMax::affinageV4()
{
    for (int row = 1; row < _rows-1; row++)
    {
        for(int col = 1; col < _cols-1; col++ )
        {
            if(_maxGradients[row][col].valS == 255)
            {
                if((_maxGradients[row-1][col].valS == 255)&&(_maxGradients[row][col+1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row-1][col].valS == 255)&&(_maxGradients[row][col-1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row+1][col].valS == 255)&&(_maxGradients[row][col-1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}
                if((_maxGradients[row+1][col].valS == 255)&&(_maxGradients[row][col+1].valS == 255))
                    {_maxGradients[row][col].valS = 0;}

            }
        }
    }
}

void GradientMapMax::resize()
{
    _maxGradients.resize(_rows);
    for(int i = 0; i < _rows; i++)
        _maxGradients[i].resize(_cols);
}

bool GradientMapMax::testeVoisin(int x, int y)
{
    for (int i= x-1; i <= x+1; i++)
    {
        for(int j=y-1; j <= y+1; j++)
        {
            if(_maxGradients[i][j].valS == 255) return true;
        }
    }
    return false;
}

void GradientMapMax::seuillageBas(float seuilB)
{
    for(int row=0; row < _rows; row++)
    {
        for(int col=0; col < _cols; col++)
        {
            if(_maxGradients[row][col].val > seuilB)//ou egale
                if(testeVoisin(row, col) == true)
                    _maxGradients[row][col].valS = 255;
        }
    }
}

void GradientMapMax::sauveGradient(string chemainDeSauve ) const
{
    ofstream newMap(chemainDeSauve.c_str());

    if (newMap)
    {
        for(int i = 60; i < 110; i++)
        {
            for(int j = 60; j < 110; j++)
            {
                newMap << _maxGradients[i][j].valS << " ";
            }
            newMap << endl;
        }


    }else printf("ERREUR: Impossible d'ouvrir le fichier3 \n");

    newMap.close();
}
