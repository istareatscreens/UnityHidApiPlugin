#ifndef CONNECTION_PROPERTIES_H
#define CONNECTION_PROPERTIES_H

#include <string>

/*
ADX IO4
    product_id = 0x0021
    vendor_id = 0x0CA3
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
  int product_id = 0x0021;
  int vendor_id = 0x0CA3;
  int buffer_size = 64;
  int polling_rate_ms = -1; // not implemented
};

#endif