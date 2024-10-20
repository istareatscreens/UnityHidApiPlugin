#ifndef CONNECTION_PROPERTIES_H
#define CONNECTION_PROPERTIES_H

#include <string>

/*
        // map front buttons
        if adx_in[4] == 1 {btns[0] |= 0x01;} // BTN1
        if adx_in[3] == 1 {btns[0] |= 0x02;} // BTN2
        if adx_in[2] == 1 {btns[0] |= 0x04;} // BTN3
        if adx_in[1] == 1 {btns[0] |= 0x08;} // BTN4
        if adx_in[8] == 1 {btns[0] |= 0x10;} // BTN5
        if adx_in[7] == 1 {btns[0] |= 0x20;} // BTN6
        if adx_in[6] == 1 {btns[0] |= 0x40;} // BTN7
        if adx_in[5] == 1 {btns[0] |= 0x80;} // BTN8

        // map side buttons
        if adx_in[9]  == 1 {btns[1] |= 0x01;} // SELECT (UP)
        if adx_in[10] == 1 {btns[2] |= 0x01;} // TEST (O1)
        if adx_in[11] == 1 {btns[2] |= 0x02;} // SERVICE (DOWN)
        if adx_in[12] == 1 {btns[2] |= 0x04;} // COIN (O2)
*/
struct ConnectionProperties
{
    int product_id = 0x0021;
    int vender_id = 0x0CA3;
};

#endif