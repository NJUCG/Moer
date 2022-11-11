/**
 * @file EntityFactory.h
 * @author JunPing.Yuan
 * @brief create Entities from json or others
 * @version 0.1
 * @date 2022-09-05
 *
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 *
 */
#pragma  once

#include "Entity.h"

class Scene;

namespace EntityFactory {

    std::shared_ptr < Entity > LoadEntityFromJson(const Json & json) ;

    std::vector<std::shared_ptr < Entity >> LoadEntityListFromJson(const Json & json,Scene & scene);
}