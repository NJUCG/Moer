/**
 * @file MaterialFactory.h
 * @author JunPing.Yuan
 * @brief A factory to load Materials from json
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma once
#include "Material.h"
#include "CoreLayer/Adapter/JsonUtil.h"

class Scene;
namespace MaterialFactory {

    std::shared_ptr < Material > LoadMaterialFromJson(const Json json) ;
    std::unordered_map < std::string, std::shared_ptr < Material>> LoadMaterialMapFromJson(const Json & json,const Scene & scene) ;
}