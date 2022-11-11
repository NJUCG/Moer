/**
 * @file FileUtils.h
 * @author JunPing Yuan
 * @brief 
 * @version 0.1
 * @date 2022/9/27
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma  once

#include <string>

namespace FileUtils{
    void setWorkingDir(const std::string & _workingDir);
    std::string getWorkingDir();
    std::string getFilePath(const std::string & path,const std::string & suffix,bool overwrite);
}
