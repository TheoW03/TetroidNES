#ifndef UTIL_H
#define UTIL_H

inline QString num_to_hexa(uint16_t num)
{
    return QString::number(num, 16).toUpper();
}

inline QString num_to_hexa(uint8_t num)
{
    return QString::number(num, 8).toUpper();
}

#endif // UTIL_H