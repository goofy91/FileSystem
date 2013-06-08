#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "FileSystem.h";
#include "FlashDriver.h";
static FlashDriver& fd=(FlashDriver::instance());
FileSystem::FileSystem(){
}
int FileSystem::open(const char* filename, int flags, int size){
    if(flags==O_CREAT){//we have to create the file
        //first let's check the filename size:
        int i;
        Header newHeader;
        for(i=0;i<FS_FILE_NAME_MAX_LENGHT && filename[i] !='\0';i++)
            newHeader.filename[i]=filename[i];
        for(;i<FS_FILE_NAME_MAX_LENGHT;i++)
            newHeader.filename[i]='\0';
        if(newHeader.filename[FS_FILE_NAME_MAX_LENGHT-1]!='\0'){
            errno=-ENAMETOOLONG;
            return -1;//filename too long
        }
        //TODO: check if there's enought space!
        //TODO: check if there's a file with the same name!
        //ok, all checked, let's write it down (FS without wholes!!!! ALL FILES ARE CONTIGUOS)
        //first set up the header
        newHeader.next=FS_BLANK;//newest file has next field with all 1
        newHeader.size=size+ALIGMENT-size%ALIGMENT;
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
            //now write the pointer of the new file (which is up on the old one)
            fd.write(address,address+(*(unsigned int *)((unsigned int)address+sizeof(unsigned int))+sizeof(Header)));
            address=*(unsigned int *)address;
        }
        //now address points to the first free area:
        return writeHeader(address,newHeader);
    }
    else if(flags==O_RDONLY){//we have to read the file
        //ensure the lenght of filename
        if(strlen(filename)>FS_FILE_NAME_MAX_LENGHT){
            errno=-ENAMETOOLONG;
            return -1;
        }
        //search for this file address:
        unsigned int address=getAddress(filename);
        if(address==FS_BLANK){//if the filename doesn't exist
            errno=-ENOENT;
            return -1;
        }
        return  address;
        
    }
    return -1;
}

int FileSystem::lstat(char* filename, struct stat* buf){
    //check the lenght
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
    unsigned int address=FS_ADDR_ROOT;//set the address to the root of the FS
    Header header;
    do{
        header=readHeader(*(unsigned int *)address);
        if(strcmp(filename,header.filename)==0)
            return *(unsigned int *)address;
    }while((*(unsigned int *)address)!=FS_BLANK);
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
    //there's the root, so point to the first file:
    address=*(unsigned int *)address;
    Header header;
    if((*(unsigned int *)address)==FS_BLANK){//there's only one file!
        header=readHeader(address);
        if(strcmp(filename,header.filename)==0){
            //if is the file to delete then delete only the root
            if(fd.erase(fd.getSectorNumber(FS_ADDR_ROOT))==false){
                errno=-EIO;
                return -1;
            }
            return 0;//there was only one file, we have delete the sector containing the root
        }
        else{
            errno=-ENOENT;
            return -1;//there's no such file
        }
    }
    address=getAddress(filename);
    if(address==FS_BLANK){//if the filename doesn't exist
        errno=-ENOENT;
        return -1;
    }
    //TODO!!! check the permissions!!!
    unsigned int sector[SECTOR_SIZE];
    header=readHeader(address);
    header.next=*(unsigned int *)address;
    //save the sector
    for(unsigned int i=0;i<SECTOR_SIZE;i++){
        sector[i]=*(unsigned int *)((address & ADDR_FLASH_SECTOR_MASK)+sizeof(unsigned int)*i);
    }
    
    //erase sector!
    if(fd.erase(fd.getSectorNumber(address))==false){
        errno=-EIO;
        return -1;
    }
    for(unsigned int i=0;i<SECTOR_SIZE;i++){
        if((address & ADDR_FLASH_SECTOR_MASK)+sizeof(unsigned int)*i!=address){
                fd.write((address & ADDR_FLASH_SECTOR_MASK)+sizeof(unsigned int)*i,sector[i]);
        }
        else{
            fd.write(address,header.next);
        }
    }
    
    return 1;
    
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

