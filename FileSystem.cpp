#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "FileSystem.h";
#include "util/util.h"
static FlashDriver& fd=(FlashDriver::instance());
FileSystem::FileSystem(){
}
bool FileSystem::checkBlank(unsigned int from,unsigned int to){
    while(from<=to){
        if(*((unsigned int *)from)!=FS_BLANK)
            return false;
        from+=sizeof(unsigned int);
    }
    return true;
}
int FileSystem::open(const char* filename, int flags, int size){
     //first of all check the length of the filename
     if(strlen(filename)>FS_FILE_NAME_MAX_LENGHT){
        errno=-ENAMETOOLONG;
        return -1;
    }
    if(flags==(O_CREAT | O_WRONLY)){//create and write
        int i;
        Header newHeader;
        for(i=0;i<FS_FILE_NAME_MAX_LENGHT && filename[i] !='\0';i++)
            newHeader.filename[i]=filename[i];
        for(;i<FS_FILE_NAME_MAX_LENGHT;i++)
            newHeader.filename[i]='\0';
        //(FS without wholes!!!! ALL FILES ARE CONTIGUOS)
        //first set up the header
        newHeader.next=FS_BLANK;//newest file has next field with all 1
        newHeader.size=size+(ALIGMENT-(size%ALIGMENT));
        newHeader.flag1=0xABCDEF12;//???TOASK
        newHeader.flag2=0x12345678;//???TOASK
        //header done
        unsigned int address=FS_ADDR_ROOT;//set the address to the root of the FS
        if((*(unsigned int *)address)==FS_BLANK){//if there isn't the root, create it:
            address+=sizeof(unsigned int);
            fd.write(FS_ADDR_ROOT,address);//the root points to the upper block of 32 bits
        }
        else{//if there's the root in the FS, then:
            if(findFile(filename)==true){//if the file already exist
                errno=-EEXIST;
                return -1;
            }
            while((*(unsigned int *)address)!=FS_BLANK){//reach the newest file in the list
                address=*(unsigned int *)address;
            }
            Header header=readHeader(address);
            //check if there's space upon the newest file!
            if(((header.size+address+sizeof(Header))+newHeader.size+sizeof(Header)<FS_ADDR_ROOF)
                    && checkBlank(header.size+address+sizeof(Header),newHeader.size+sizeof(Header)+address) ){
                //if we haven't reach the roof yet and
                //if the space is actually blank
                //now write the pointer of the new file (which is up on the old one)
                fd.write(address,address + header.size+sizeof(Header));
                address=*(unsigned int *)address;
            }
            else{//find wholes!!!!!
                //TODO!!!!!!
                //REMEBER: save the address to put the new file in address!!!!
            }
        }
        //now address points to the first free area:
        return writeHeader(address,newHeader);
    }
    else if(flags==O_RDONLY){//we have to read the file
        //search for this file address:
        unsigned int address=getAddress(filename);
        if(address==FS_BLANK){//if the filename doesn't exist
            errno=-ENOENT;
            return -1;
        }
        return  address;
        
    }
    else if(flags== O_WRONLY){//only write without creating it.
        unsigned int address=getAddress(filename);
        if(address==FS_BLANK){//if the filename doesn't exist
            errno=-ENOENT;
            return -1;
        }
        //search the first space blank of the file:
        Header header=readHeader(address);
        address+=sizeof(Header);//point to the first byte of the file
        
        return open(filename,O_WRONLY | O_CREAT,size); //TO ASK!!!!!!!
    }
    return -1;
}

int FileSystem::lstat(char* filename, struct stat* buf){
    //check the length
    if(strlen(filename)>FS_FILE_NAME_MAX_LENGHT){
            errno=-ENAMETOOLONG;
            return -1;
    }
    //find the file:
    unsigned int address=getAddress(filename);
    if(address==FS_BLANK){
        errno=-ENOENT;
        return -1;
    }
    Header header=readHeader(address);
    buf->st_nlink=1;
    //TO ASK!!!!!!! UID E FLAGS!!!
    /*buf->st_gid;
    buf->st_uid;
    buf->st_mode;*/
    buf->st_size=header.size;
    buf->st_blksize=512;
    int blocks=(((header.size)/(buf->st_blksize))+((buf->st_blksize)-1))/(buf->st_blksize);
    blocks= blocks * buf->st_blksize;
    buf->st_blocks= blocks;//round up!!
    //all other field must be cleared!
    buf->st_atime=0;
    buf->st_ctime=0;
    buf->st_ino=0;
    buf->st_mode=0;
    buf->st_rdev=0;
    buf->st_mtime=0;
    buf->st_spare1=0;
    buf->st_spare2=0;
    buf->st_spare3=0;
    buf->st_spare4[0]=0;
    buf->st_spare4[1]=0;
    return 0;
} 

/**
  * ATTENTION size of the filename field must be divisible for 2
  * @param address
  * @param header
  * @return 
  */
unsigned int FileSystem::writeHeader(unsigned int address,Header header){
    fd.write(address,header.next);
    address+=sizeof(unsigned int);
    fd.write(address,header.size);
    address+=sizeof(unsigned int);
    fd.write(address,header.flag1);
    address+=sizeof(unsigned int);
    fd.write(address,header.flag2);
    address+=sizeof(unsigned int);
    short a=0;
    for(int i=0;i<FS_FILE_NAME_MAX_LENGHT;i++){
        a=header.filename[i] | (header.filename[++i] <<8);
        fd.write(address,(short)a);
        address+=sizeof(short);
    }
    return address;
}
Header FileSystem::readHeader(unsigned int address){
    Header header;
    header.next=*(unsigned int *)address;
    address+=+sizeof(unsigned int);
    header.size=*(unsigned int *)(address);
    address+=sizeof(unsigned int);
    header.flag1=*(unsigned int*)(address);
    address+=sizeof(unsigned int);
    header.flag2=*(unsigned int*)(address);
    address+=sizeof(unsigned int);
    //get the filename:
    short a;
    int i=0;
    while(i<FS_FILE_NAME_MAX_LENGHT){
        a=*(short *)(address);
        header.filename[i++]=(char)a;
        header.filename[i++]=(char)(a>>8);
        address+=sizeof(short);
    }
    return header;
    
}
void FileSystem::clearFS(){
    //let's write FFFF on the whole FS
    int i;
    for(i=FS_SECTOR_ROOT;i<=FS_SECTOR_ROOF;i++)
        fd.erase(i);
}

FileSystem& FileSystem::instance(){
        /*if(fs==0) fs=new FileSystem();
    return fs;*/
    static FileSystem singleton;
    return singleton;
}

unsigned int FileSystem::getAddress(const char* filename){
    Header header;
    if(*(unsigned int *)(FS_ADDR_ROOT)==FS_BLANK)//if there's no root!
        return FS_BLANK;
    unsigned int address=*(unsigned int *)(FS_ADDR_ROOT);
    do{
    header=readHeader(address);//header of the first file
    if(strcmp(filename,header.filename)==0)
            return address;
    address=(header.next);
    }while(address!=FS_BLANK);
    return FS_BLANK;
}
unsigned int FileSystem::getAddressBefore(const char* filename){
    Header header;
    if(*(unsigned int *)(FS_ADDR_ROOT)==FS_BLANK)//if there's no root!
        return FS_BLANK;
    unsigned int address=FS_ADDR_ROOT;
    do{
        header=readHeader(*(unsigned int *)address);
        if(strcmp(filename,header.filename)==0)
            return address;
        address=*(unsigned int *)address;
    }while(address!=FS_BLANK);
    return FS_BLANK;
    
}
int FileSystem::unlink(const char* filename){
    //is pathname empty?
    if(filename[0]=='\0'){
        errno=-ENOENT;
        return -1;
    }
    //check if there's the root
    unsigned int address=FS_ADDR_ROOT;//set the address to the root of the FS
    if((*(unsigned int *)address)==FS_BLANK){//if there's no root, there's no file to unlink
        errno=-ENOENT;
        return -1;
    }
    //check the file if exist!
    if(findFile(filename)==false){//if the filename doesn't exist
        errno=-ENOENT;
        return -1;
    }
    //there's the root, so point to the first file:
    address=*(unsigned int *)address;
    Header header;
    if((*(unsigned int *)address)==FS_BLANK){//there's only one file!
        header=readHeader(address);
            //if is the file to delete then delete only the root
            if(fd.erase(fd.getSectorNumber(FS_ADDR_ROOT))==false){
                errno=-EIO;
                return -1;
            }
            //if the file to unlink and the root aren't in the same sector, then erase both sectors
            if((address & ADDR_FLASH_SECTOR_MASK) != FS_ADDR_ROOT){
                if(fd.erase(fd.getSectorNumber(address & ADDR_FLASH_SECTOR_MASK))==false){
                        errno=-EIO;
                        return -1;
                }
            }
            return 0;//there was only one file, we have delete the sector containing the root
        
    } 
    address=getAddressBefore(filename);
    unsigned int addressFileToDelete=*(unsigned int *)address;
    Header headerFileToDelete=readHeader(addressFileToDelete);

    //TODO!!! check the permissions!!!
    if((addressFileToDelete & ADDR_FLASH_SECTOR_MASK )==(address & ADDR_FLASH_SECTOR_MASK)){
        //if the file before and the file to unlink are in the same sector then:
        //erase the buffer sector:
        if(fd.erase(FS_SECTOR_BUFFER)==false){
            errno=-EIO;
            return -1;
        }
        //copy the whole sector (except the file to delete) to the buffer sector:
        unsigned int i=0;
        while(i<SECTOR_SIZE){
            if(i+(address & ADDR_FLASH_SECTOR_MASK)==addressFileToDelete)
                i+=headerFileToDelete.size+sizeof(header);
            else if(i+(address & ADDR_FLASH_SECTOR_MASK)==address){
                if(fd.write(FS_ADDR_BUFFER+i,headerFileToDelete.next)==false){
                    errno=-EIO;
                    return -1;
                }
                i+=ALIGMENT;
            }
            else{
                if(fd.write(FS_ADDR_BUFFER+i,*(unsigned int *)(i+(address & ADDR_FLASH_SECTOR_MASK)))==false){
                    errno=-EIO;
                    return -1;
                }
                i+=ALIGMENT;
            }
        }
        
    }
    else{//if the file to unlink and the file before aren't in the same sector:
        //save the whole sector where the file before is
        unsigned int i=0;
        while(i<SECTOR_SIZE){
            if(i+(address & ADDR_FLASH_SECTOR_MASK)==address){
                if(fd.write(FS_ADDR_BUFFER+i,headerFileToDelete.next)==false){
                    errno=-EIO;
                    return -1;
                }
                i+=ALIGMENT;
            }
            else{
                if(fd.write(FS_ADDR_BUFFER+i,*(unsigned int *)(i+(address & ADDR_FLASH_SECTOR_MASK)))==false){
                    errno=-EIO;
                    return -1;
                }
                i+=ALIGMENT;
            }
        }
    }
    //erase sector!
    if(fd.erase(fd.getSectorNumber(address))==false){
        errno=-EIO;
        return -1;
    }
    //and then copy it back!
    if(fd.write(address & ADDR_FLASH_SECTOR_MASK,(char *)(FS_ADDR_BUFFER),SECTOR_SIZE)==false){
        errno=-EIO;
        return -1;
    }
    return 0;
    
}
/*
 * Tested!!
 */
bool FileSystem::findFile(const char *filename){
    unsigned int address=FS_ADDR_ROOT;//set the address to the root of the FS
    Header header;
    while((*(unsigned int *)address)!=FS_BLANK){
        address=*(unsigned int *)address;
        header=readHeader(address);
        if(strcmp(filename,header.filename)==0)
            return true;
    }
    return false;
}