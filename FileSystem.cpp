#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include "FileSystem.h";
#include "util/util.h";
#include <cstdio>
#include <list>//TO DELETE

#include "miosix.h"//TO DELETE

static FlashDriver& fd = (FlashDriver::instance());

FileSystem::FileSystem() {
}

bool FileSystem::checkBlank(unsigned int from, unsigned int to) {
    while (from <= to) {
        if (*((unsigned int *) from) != FS_BLANK)
            return false;
        from += sizeof (unsigned int);
    }
    return true;
}

bool FileSystem::isValid(unsigned int address) {
    return (*((char *) (address + sizeof (Header) - FS_FILE_NAME_MAX_LENGHT)) == 0x00 ? false : true);
    /*  if(readHeader(address).filename[0]==0x00)
          return false;
      return true;*/
}

File* FileSystem::open(const char* filename, int flags, int size) {
    //first of all check the length of the filename
    if (strlen(filename) > FS_FILE_NAME_MAX_LENGHT) {//filename too long
        return NULL;
    }
    if (filename[0] == 0x00) {//invalid first character
        return NULL;
    }
    if (flags == (O_CREAT | O_WRONLY)) {//create and write
        int i;
        Header newHeader;
        for (i = 0; i < FS_FILE_NAME_MAX_LENGHT && filename[i] != '\0'; i++)
            newHeader.filename[i] = filename[i];
        for (; i < FS_FILE_NAME_MAX_LENGHT; i++)
            newHeader.filename[i] = '\0';
        newHeader.next = size + (sizeof(Header) - (size % sizeof(Header))) + sizeof (Header);
        newHeader.uid = getuid();
        newHeader.gid = getgid();
        unsigned int address = fd.getStartSector(); //set the address to the root of the FS
        if (findFile(filename) == true) {//if the file already exist
            return NULL;
        }
        while ((*(unsigned int *) address) != FS_BLANK) {//reach the newest file in the list
            address = *(unsigned int *) address;
        }
        //now check if there's enough space to create the file up on the highest file saved
        if (newHeader.next + address < fd.getLastAddress()) {
            //YES! there's space
            newHeader.next += address;
            //now write it!
            if (writeHeader(address, newHeader) != sizeof (Header))//if writing fails
                return NULL;
            return new File(address + sizeof (Header), size);
        } else {
            //NO! we have to find holes
            address = fd.getStartSector(); //set the address to the root of the FS
            unsigned int sizeHole = 0;
            std::vector<unsigned int> holes;
            while ((*(unsigned int *) address) != FS_BLANK && address < fd.getLastAddress()) {
                if (isValid(address) == false) {//if it's a whole!
                    sizeHole = (*(unsigned int *) address) - address;
                    if (sizeHole >= newHeader.next && address + sizeHole < fd.getLastAddress()) { // if there's enough space
                        holes.push_back(address);
                    }
                }
                address = *(unsigned int *) address;
            }
            //finish scanning the FS. 
            //All the possible holes are saved, now flip a coin to insert the file in a random holes

            //To ask generate seed number

            address = holes.at((int) (rand() % ((int) holes.capacity())));
            iprintf("\n\naddress %x\n\n", address);
            iprintf("\n\nsizeHole %x\n\n", sizeHole);
            bool gg = isValid((unsigned int) 0x08080060);
            if (gg == true)
                iprintf("\nIsValid true \n");
            iprintf("\nnewHeader.next %d\n", newHeader.next);
            //save the sector where is the file to delete in a sector used as a buffer
            unsigned int i = 0;
            newHeader.next = readHeader(address).next;

            if (fd.erase(FS_SECTOR_BUFFER) == false) {
                return NULL;
            }
            if ((address & ADDR_FLASH_SECTOR_MASK) == ((newHeader.next) & ADDR_FLASH_SECTOR_MASK)) {
                //the whole file to open is in the same sector
                while (i < SECTOR_SIZE) {
                    if (i + (address & ADDR_FLASH_SECTOR_MASK) == address) {
                        //write the new header
                        if (writeHeader(fd.getBufferSector() + i, newHeader) != sizeof (Header)) {
                            return NULL;
                        }
                        i = newHeader.next - (address & ADDR_FLASH_SECTOR_MASK);
                    } else {
                        if (fd.write(fd.getBufferSector() + i, *(unsigned int *) (i + (address & ADDR_FLASH_SECTOR_MASK))) == false) {
                            return NULL;
                        }
                        i += ALIGMENT;
                    }
                }

                //erase the sector:
                if (fd.erase(fd.getSectorNumber(address)) == false) {
                    return NULL;
                }
                //and then copy it back!
                if (fd.write(address & ADDR_FLASH_SECTOR_MASK, (char *) (fd.getBufferSector()), SECTOR_SIZE) == false) {
                    return NULL;
                }
            } else {
                //the file to open is between two sectors
                while (i < SECTOR_SIZE) {
                    if (i + (address & ADDR_FLASH_SECTOR_MASK) == address) {
                        //write the new header
                        if((writeHeader(fd.getBufferSector() + i, newHeader))==false)
                            return NULL;
                        break;
                    } else {
                        if (fd.write(fd.getBufferSector() + i, *(unsigned int *) (i + (address & ADDR_FLASH_SECTOR_MASK))) == false) {
                            return NULL;
                        }
                        i += ALIGMENT;
                    }
                }
                //erase the sector:
                if (fd.erase(fd.getSectorNumber(address)) == false) {
                    return NULL;
                }
                //and then copy it back!
                if (fd.write(address & ADDR_FLASH_SECTOR_MASK, (char *) (fd.getBufferSector()), SECTOR_SIZE) == false) {
                    return NULL;
                }

                //erase the buffer
                if (fd.erase(FS_SECTOR_BUFFER) == false) {
                    return NULL;
                }
                //if all the header is in the buffer
                //copy the other sector in the buffer starting from newHeader.next
                if (fd.write(fd.getBufferSector()+(newHeader.next - (newHeader.next & ADDR_FLASH_SECTOR_MASK)),
                        (char *) (newHeader.next),
                        SECTOR_SIZE - (newHeader.next - (newHeader.next & ADDR_FLASH_SECTOR_MASK))) == false) {
                    return NULL;
                }

                //erase the sector:
                if (fd.erase(fd.getSectorNumber(newHeader.next)) == false) {
                    return NULL;
                }
                //then copy it back:
                if (fd.write(newHeader.next & ADDR_FLASH_SECTOR_MASK, (char *) (fd.getBufferSector()), SECTOR_SIZE) == false) {
                    return NULL;
                }

            }
            return new File(address + sizeof (Header), size);

        }
    } else if (flags == O_RDONLY) {//we have to read the file
        //search for this file address:
        unsigned int address = getAddress(filename);
        if (address == FS_BLANK) {//if the filename doesn't exist
            return NULL;
        }
        return new File(address + sizeof (Header), size);
    }
    return NULL;
}

int FileSystem::lstat(char* filename, struct stat* buf) {
    //check the length
    if (strlen(filename) > FS_FILE_NAME_MAX_LENGHT) {
        return -ENAMETOOLONG;
    }
    //find the file:
    unsigned int address = getAddress(filename);
    if (address == FS_BLANK) {
        return -ENOENT;
    }
    Header header = readHeader(address);
    buf->st_nlink = 1;
    memset(buf, 0, sizeof (struct stat));
    buf->st_uid = header.uid;
    buf->st_gid = header.gid;
    buf->st_size = header.size;
    buf->st_blksize = 512;
    int blocks = (buf->st_size + buf->st_blksize - 1) / buf->st_blksize;
    blocks = blocks * buf->st_blksize;
    buf->st_blocks = blocks; //round up!!
    return 0;
}

bool FileSystem::writeHeader(unsigned int address, Header header) {
    if (fd.write(address, header.next) == false)
        return false;
    address = offsetof(Header, next);
    if (fd.write(address, header.size) == false)
        return false;
    address = offsetof(Header, size);
    if (fd.write(address, (short) header.uid) == false)
        return false;
    address = offsetof(Header, uid);
    if (fd.write(address, (short) header.gid) == false)
        return false;
    address = offsetof(Header, gid);
    if (fd.write(address, header.filename, FS_FILE_NAME_MAX_LENGHT) == false)
        return false;
    address = offsetof(Header, filename);
    return true;
}

Header FileSystem::readHeader(unsigned int address) {
    Header header;
    header.next = *(unsigned int *) address;
    address += +sizeof (unsigned int);
    header.size = *(unsigned int *) (address);
    address += sizeof (unsigned int);
    header.uid = *(uid_t*) (address);
    address += sizeof (uid_t);
    header.gid = *(gid_t*) (address);
    address += sizeof (gid_t);
    //get the filename:
    short a;
    int i = 0;
    while (i < FS_FILE_NAME_MAX_LENGHT) {
        a = *(short *) (address);
        header.filename[i++] = (char) a;
        header.filename[i++] = (char) (a >> 8);
        address += sizeof (short);
    }
    return header;

}

void FileSystem::clearFS() {
    //let's write FFFF on the whole FS
    int i;
    for (i = FS_SECTOR_ROOT; i <= FS_SECTOR_ROOF; i++)
        fd.erase(i);
}

FileSystem& FileSystem::instance() {
    /*if(fs==0) fs=new FileSystem();
return fs;*/
    static FileSystem singleton;
    return singleton;
}

unsigned int FileSystem::getAddress(const char* filename) {
    Header header;
    if (*(unsigned int *) (fd.getStartSector()) == FS_BLANK)//if there's no root!
        return FS_BLANK;
    unsigned int address = *(unsigned int *) (fd.getStartSector());
    do {
        header = readHeader(address); //header of the first file
        if (strcmp(filename, header.filename) == 0)
            return address;
        address = (header.next);
    } while (address != FS_BLANK);
    return FS_BLANK;
}

int FileSystem::unlink(const char* filename) {
    //is pathname empty?
    if (filename[0] == '\0') {
        return -ENOENT;
    }
    //check the file if exist!
    if (findFile(filename) == false) {//if the filename doesn't exist
        return -ENOENT;
    }
    //erase it by writing 0x00 in the first char of the filename
    unsigned int address = getAddress(filename);
    Header header = readHeader(address);
    if (fd.write(address + sizeof (Header) - FS_FILE_NAME_MAX_LENGHT, (char) 0x00) == false)
        return -EIO;
    return 0;

}

/*
 * Tested!!
 */
bool FileSystem::findFile(const char *filename) {
    unsigned int address = fd.getStartSector(); //set the address to the root of the FS
    Header header;
    while (*(unsigned int *) address != FS_BLANK && address < fd.getLastAddress()) {
        header = readHeader(address);
        if (strcmp(filename, header.filename) == 0)
            return true;
        address = *(unsigned int *) address;
    }
    return false;
}