#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>

class Gradient
{
private:

    std::vector<double> _values;

public:

    Gradient() {}

    void resize(int dims);

    double getValueAt(int pos) const { return _values[pos]; }
    void setValueAt(int pos, double value) { _values[pos] = value; }
};

#endif // GRADIENT_H
