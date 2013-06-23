/* 
 * File:   File.h
 * Author: goofy
 *
 * Created on June 5, 2013, 7:42 PM
 */

#ifndef FILE_H
#define	FILE_H/*
#include "FlashDriver.h"
#include <unistd.h>


class File{
public:
    ssize_t write(const void *buff,size_t count);
    ssize_t read(void *buf,size_t count);
    int fstat(struct stat *buf);
    off_t lseek(off_t offset,int whence);
    unsigned int getBasePointer();
    File(unsigned int basePointer,size_t lengh);

private:
    unsigned int basePointer;
    int fp;
    size_t lenght;
    
};*/

#endif	/* FILE_H */

