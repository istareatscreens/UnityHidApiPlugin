#ifndef CONNECTION_PROPERTIES_H
#define CONNECTION_PROPERTIES_H

#include <string>

/*
ADX IO4
    vendor_id = 0x0CA3
    product_id = 0x0021
    buffer_size = 64
*/
/*
ADX HID
  vendor_id = 0x2e3c
  product_id = 0x5750
  buffer_size = 64 ?
*/

struct ConnectionProperties
{
  int vendor_id = 0x0CA3;
  int product_id = 0x0021;
  // zero index inclusive [1, 4] in a bit string of 00000 would truncate 0000
  // Going outside of bounds is not allowed!
  int read_left = 0;
  int read_right = 0;
  int buffer_size = 64;
  int polling_rate_ms = -1; // not implemented
};

#endif