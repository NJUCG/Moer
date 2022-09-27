/**
 * @file JsonUtil.hpp
 * @author JunPing Yuan
 * @brief  contains nlohmann json and some useful json functions
 * @version 0.1
 * @date 2022/9/7
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma  once
#include "nlohmann/json.hpp"
#include "CoreLayer/ColorSpace/Color.h"
#include "../Geometry/Geometry.h"

typedef nlohmann::json Json;

template <class T>
inline T getOptional(const Json &j, std::string field, T default_value)
{
    T ret = default_value;
    if (j.find(field) != j.end())
    {
        ret = j.at(field).get<T>();
    }
    return ret;
}

template <class T>
inline  bool containsAndGet(const Json &j, std::string field, T & value)
{
    if (j.find(field) != j.end())
    {
        value = j.at(field).get<T>();
        return true;
    }
    return false;
}



template<class T>
void from_json(const Json &j,TVector2<T> & vec2){
    if (j.type() == Json::value_t::array)
        for (int i = 0; i < 2; i++) j.at(i).get_to(vec2[i]);
    else
        for (int i = 0; i < 2; i++) j.get_to(vec2[i]);
}


template<class T>
void from_json(const Json &j,TVector3<T> & vec3){
    if (j.type() == Json::value_t::array)
        for (int i = 0; i < 3; i++) j.at(i).get_to(vec3[i]);
    else
        for (int i = 0; i < 3; i++) j.get_to(vec3[i]);
}

template<class T>
void from_json(const Json &j,TPoint2<T> & point2){
    if (j.type() == Json::value_t::array)
        for (int i = 0; i < 2; i++) j.at(i).get_to(point2[i]);
    else
        for (int i = 0; i < 2; i++) j.get_to(point2[i]);
}


template<class T>
void from_json(const Json &j,TPoint3<T> & point3){
    if (j.type() == Json::value_t::array)
        for (int i = 0; i < 3; i++) j.at(i).get_to(point3[i]);
    else
        for (int i = 0; i < 3; i++) j.get_to(point3[i]);
}



//void from_json(const Json &j,RGB3 rgb3){
//    if (j.type() == Json::value_t::array)
//        for (int i = 0; i < 3; i++) j.at(i).get_to(rgb3[i]);
//    else
//        for (int i = 0; i < 3; i++) j.get_to(rgb3[i]);
//}
