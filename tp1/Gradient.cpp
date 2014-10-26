#include "Gradient.h"

#include <cmath>

Gradient::Gradient(int size)
    :_values(size)
{
}

int Gradient::getMaxValueDirection() const
{
    int valueCount;
    int maxValueIndex;

    maxValueIndex = 0;
    valueCount = _values.size();

    for (int i = 0; i < valueCount; ++i)
        if (std::abs(_values[i]) > std::abs(_values[maxValueIndex]))
            maxValueIndex = i;

    return maxValueIndex;
}
