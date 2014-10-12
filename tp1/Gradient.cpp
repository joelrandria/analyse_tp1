#include "Gradient.h"

void Gradient::resize(int size)
{
    _values.resize(size);
}

int Gradient::getMaxValueDirection() const
{
    int valueCount;
    int maxValueIndex;

    maxValueIndex = 0;
    valueCount = _values.size();

    for (int i = 0; i < valueCount; ++i)
        if (_values[i] > _values[maxValueIndex])
            maxValueIndex =i;

    return maxValueIndex;
}
