#include <sys/stat.h>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "FileUtils.h"

inline bool fileExists (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

template<typename ... Args>
std::string stringFormat( const char * format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format, args ... ) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format, args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

namespace FileUtils{
    static std::string workingDir;
    void setWorkingDir(const std::string & _workingDir){
        workingDir = _workingDir;
    }
    std::string getWorkingDir(){
        return workingDir;
    }

    std::string getFullPath(const std::string &path) {
        std::string realPath = path;
#if _WIN32
        std::replace(realPath.begin(), realPath.end(), '/', '\\');
#endif
        return workingDir+realPath;
    }

    std::string getFilePath(const std::string & path,const std::string & suffix,bool overwrite){
        std::string destPath = path + "." +suffix;
        if(overwrite)
            //return std::move(destPath);
            return destPath;
        int count = 1;
        while( fileExists(destPath)){
            destPath = stringFormat("%s%d.%s",path.c_str(),count++,suffix.c_str());
        }
        //return std::move(destPath);
        return destPath;
    }
}

std::string FileUtils::getFileExtension(const std::string & path) {
    int dotIdx = path.find(".");
    if(dotIdx == -1)
        return "";
    return path.substr(dotIdx+1,path.size());
}