#include "JsonUtil.h"

void from_json(const Json & j, RGB3 & rgb3) {
    if ( j.type() == Json::value_t::array )
        for ( int i = 0 ; i < 3 ; i ++ ) j.at(i).get_to(rgb3[i]);
    else
        for ( int i = 0 ; i < 3 ; i ++ ) j.get_to(rgb3[i]);
}

void from_json(const Json & j, Spectrum & spectrum) {
    RGB3 rgb3 = j;
    spectrum = Spectrum(rgb3);
}
