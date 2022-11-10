#include "JsonUtil.h"
#include "CoreLayer/ColorSpace/Color.h"

void from_json(const Json & j, RGB3 & rgb3) {
    if ( j.type() == Json::value_t::array )
        for ( int i = 0 ; i < 3 ; i ++ ) j.at(i).get_to(rgb3[i]);
    else
        for ( int i = 0 ; i < 3 ; i ++ ) j.get_to(rgb3[i]);
}

void from_json(const Json & j, RGBSpectrum & spectrum) {
    if ( j.is_number() )
        spectrum = Spectrum(j.get < double >());
    else if ( j.is_array() ) {
        for ( int i = 0 ; i < j.size() ; i ++ )
            j.at(i).get_to(spectrum[i]);
    }
}
