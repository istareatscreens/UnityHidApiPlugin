#include "AdxIO4ButtonRing.h"
#include "ConnectionProperties.h"
#include <iostream>

bool AdxIO4ButtonRing::connect()
{
    std::cout << "HELLLLLLLLLLLLLLLLLLLOOOOOOOOOOOOO";
    struct hid_device_info *devs, *cur_dev;

    ConnectionProperties connection_properties;

    devs = hid_enumerate(connection_properties.vender_id, connection_properties.product_id);

    std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << devs->manufacturer_string;

    return true;
}

void AdxIO4ButtonRing::read()
{
    std::cout << "Example function called!" << std::endl;
}

bool AdxIO4ButtonRing::disconnect()
{
    std::cout << "Example function called!" << std::endl;
    return true;
}
