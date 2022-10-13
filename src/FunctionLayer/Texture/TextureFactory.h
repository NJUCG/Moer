/**
 * @file TextureFactory
 * @author JunPing Yuan
 * @brief  Simple texture factory class with many features to be improved。
 * @version 0.1
 * @date 2022/10/4
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include "memory"
#include "Texture.h"
namespace  TextureFactory{
    template <class T>
    std::shared_ptr<Texture<T>> LoadTexture(const Json & textureJson){
        if(textureJson.is_null()){
            return nullptr;
        }
        if(textureJson.is_number()){
            return std::make_shared <ConstantTexture<T>>(textureJson);
        }
        if(textureJson.is_array()){
            return std::make_shared <ConstantTexture<T>>(textureJson);
        }
        if(textureJson.is_string()){
            //todo
            return nullptr;
        }
        if(textureJson.is_object()){
            std::string textureType = textureJson.at("type");
            if(textureType == "checker"){
                T onColor= getOptional(textureJson,"on_color",T());
                T offColor= getOptional(textureJson,"off_color",T());
                int resU = getOptional(textureJson,"res_u",20);
                int resV = getOptional(textureJson,"res_v",20);
                //todo
               // return std::make_shared <CheckTexture<T>>(onColor,offColor,resU,resV);
            }
        }
        return nullptr;
        // return std::make_shared <ConstantTexture<T>>(defaultValue);
    }

    template <class T>
    std::shared_ptr<Texture<T>> LoadTexture(const nlohmann::json & textureJson,T defaultValue){
        if(auto texture = LoadTexture<T>(textureJson))
            return texture;
        return std::make_shared<ConstantTexture<T>>(defaultValue);
    }

    template <class T>
    std::shared_ptr<ConstantTexture<T>> LoadConstantTexture(T value){
        return std::make_shared<ConstantTexture<T>>(value);
    }
}



