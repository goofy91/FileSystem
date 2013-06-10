#include "File.h";
#include <string.h>
static FlashDriver& flashDriver=FlashDriver::instance();

static FileSystem& fileSystem=FileSystem::instance();
File::File(int basePointer,size_t lenght){
    this->lenght=lenght;
    this->fd=basePointer;
    this->basePointer=basePointer;
}

ssize_t File::write(int fd,const void *buff,size_t count){
    if(fd<(this->basePointer) || fd>(basePointer+this->lenght)){
        errno=-EBADF;
        return -1;
    }
    //TODO CHECK COUNT SIZE!!!!!
    flashDriver.write((unsigned int) fd,(char *)buff,count);
    return count;
}

ssize_t File::read(int fd, void *buf,size_t count){
    if(fd<(this->basePointer) || fd>(basePointer+this->lenght)){
        errno=-EBADF;
        return -1;
    }
    int i=0;/*
    while(i<count && fd<(this->basePointer+lenght)){
        *(char *)buf=*((char *)fd+i);
    }*/
    //TODO CHECK THE COUNT!!!!!!
    memcpy((void *)buf,(void *)fd,count);
    return i;
}

int File::fstat(int fd,struct stat *buf){
    //TODO!!
    return 0;
}

off_t lseek(int fd,off_t offset,int whence){
    if(whence==SEEK_SET){
    }
    else if(whence==SEEK_CUR){
        
    }
    else if(whence==SEEK_END){
        
    }
    return 0;
}


unsigned int File::getBasePointer(){
    return this->basePointer;
}



