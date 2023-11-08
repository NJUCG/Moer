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
#include <fstream>
#include <vector>
namespace FileUtils{
    void setWorkingDir(const std::string & _workingDir);
    std::string getWorkingDir();
    std::string getFilePath(const std::string & path,const std::string & suffix,bool overwrite);
    std::string getFileExtension(const std::string & path);
    std::string getFullPath(const std::string & path);

    template<typename T>
    static inline void streamRead(std::istream &in, T &dst)
    {
        in.read(reinterpret_cast<char *>(&dst), sizeof(T));
    }

    template<typename T>
    static inline T streamRead(std::istream  &in)
    {
        T t;
        streamRead(in, t);
        return t;
    }

    template<typename T>
    static inline void streamRead(std::istream &in, std::vector<T> &dst)
    {
        in.read(reinterpret_cast<char *>(&dst[0]), dst.size()*sizeof(T));
    }

    template<typename T>
    static inline void streamRead(std::istream &in, T * dst,size_t n)
    {
        in.read(reinterpret_cast<char *>(dst), n*sizeof(T));
    }
}

template<>
inline std::string FileUtils::streamRead<std::string>(std::istream  &in)
{
    std::string s;
    std::getline(in, s, '\0');
    return std::move(s);
}
