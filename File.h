/* 
 * File:   File.h
 * Author: goofy
 *
 * Created on June 5, 2013, 7:42 PM
 */

#ifndef FILE_H
#define	FILE_H
#include "FileSystem.h";
#include "FlashDriver.h"
#include <unistd.h>


class File{
public:
    ssize_t write(int fd,const void *buff,size_t count);
    ssize_t read(int fd, void *buf,size_t count);
    int fstat(int fd,struct stat *buf);
    off_t lseek(int fd,off_t offset,int whence);
    unsigned int getBasePointer();
    File(int basePointer,size_t lengh);

private:
    unsigned int basePointer;
    int fd;
    size_t lenght;
    Header header;
    
};

#endif	/* FILE_H */

