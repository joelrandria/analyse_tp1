#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>

class Gradient
{
private:

    std::vector<double> _values;

public:

    Gradient(){}
    Gradient(int size);

    int getMaxValueDirection() const;

    double getValueAt(int index) const { return _values[index]; }
    void setValueAt(int index, double value) { _values[index] = value; }
};

#endif // GRADIENT_H
