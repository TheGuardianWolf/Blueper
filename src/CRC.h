#pragma once

#include <stdint.h>
#include <stddef.h>

class Crc16Ccitt1021
{
public:
    Crc16Ccitt1021(uint16_t initialValue = 0);

    void Reset(uint16_t initialValue = 0);
    void Calc(uint8_t value)
    {
        m_result = uint16_t(CRC16CCITT1021_TABLE[value ^ (m_result >> 8)] ^ (m_result << 8));
    }
    uint16_t CalcBuffer(uint8_t const *pbuf, size_t len);
    uint16_t Result() const { return m_result; }

    static const size_t CRC_SIZE;

private:
    uint16_t m_result;
    static const uint16_t CRC16CCITT1021_TABLE[];
};
