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
  // This will truncate the returned by left_bytes_to_truncate and impact how
  // previous buffer is compared to current buffer.
  // Warning bytes_to_read + left_bytes_to_truncate <= buffer_size
  int buffer_size = 64;
  int left_bytes_to_truncate = 0;
  int bytes_to_read = 64;
  int polling_rate_ms = 0; // not implemented
};

#endif