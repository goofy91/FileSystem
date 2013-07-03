/***************************************************************************

 *   Copyright (C) 2013 by                                                 * 
 *      Filippo Jacolino, Tommaso Innocenti, Fernando Faenza               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/ 
#include "FileSystem.h";
#include <string.h>
static FlashDriver& flashDriver=FlashDriver::instance();

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
    this->fp +=byteToWrite;
    return byteToWrite;
}
unsigned int File::getBasePointer(){
    return this->basePointer;
}
size_t File::getSize(){
    return this->lenght;
}
ssize_t File::read(void *buf,size_t count){
    int i=0;
    i= (this->fp + count >this-> lenght) ? lenght : count;
    memcpy((void *)buf,(void *)(this->basePointer + this->fp),i);
    this->fp+=i;
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

