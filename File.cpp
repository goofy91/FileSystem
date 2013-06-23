#include "FileSystem.h";
#include <string.h>
static FlashDriver& flashDriver=FlashDriver::instance();
//static FileSystem& fileSystem=FileSystem::instance();

File::File(unsigned int basePointer,size_t lenght, uid_t uid, gid_t gid){
    this->lenght=lenght;
    this->basePointer=basePointer;
    this->fp=0;
    this->uid=uid;
    this->gid=gid;
}
ssize_t File::write(const void *buff,size_t count){
    unsigned int byteToWrite= (this->fp + count > this->lenght) ? lenght : count;
    if(flashDriver.write(this->fp+this->basePointer,(char *)buff,byteToWrite)==false)
        return -EIO;
    return byteToWrite;
}
unsigned int File::getBasePointer(){
    return this->basePointer;
}

ssize_t File::read(void *buf,size_t count){
    int i=0;
    //TODO CHECK THE COUNT!!!!!!
    i= (this->fp + count >this-> lenght) ? lenght : count;
    memcpy((void *)buf,(void *)(this->basePointer + this->fp),i);
    return i;
}

int File::fstat(struct stat *buf){
    buf->st_nlink = 1;
    memset(buf, 0, sizeof (struct stat));
    buf->st_uid = this->uid;
    buf->st_gid = this->gid;
    buf->st_size = this->lenght;
    buf->st_blksize = 512;
    int blocks = (buf->st_size + buf->st_blksize - 1) / buf->st_blksize;
    blocks = blocks * buf->st_blksize;
    buf->st_blocks = blocks; //round up!!
    return 0;
}
off_t File::lseek(off_t offset,int whence){
    if(whence==SEEK_SET){
        if(offset > this->lenght || offset<0 )
            return -1;
        this->fp=offset;
        return this->fp;
    }
    else if(whence==SEEK_CUR){
        if(offset + this->fp > this->lenght  || offset + this->fp <0 )
            return -1;
        this->fp+=offset;
        return this->fp;
    }
    else if(whence==SEEK_END){
        if(this->lenght + offset > this->lenght || this->lenght + offset <0)
            return -1;
        this->fp=this->lenght + offset;
        return this->fp;
    }
    //else: whence is not valid!
    return -EINVAL;
}

