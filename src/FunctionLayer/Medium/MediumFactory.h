/**
 * @file MediumFactory.h
 * @author JunPing Yuan
 * @brief MediumFactory
 * @version 0.1
 * @date 2022/9/26
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma  once

#include "Medium.h"
#include "CoreLayer/Adapter/JsonUtil.h"


namespace  MediumFactory{
    std::shared_ptr < Medium > LoadMediumFromJson(const Json json) ;
    std::unordered_map < std::string, std::shared_ptr < Medium>> LoadMediumMapFromJson(const Json json) ;
}
