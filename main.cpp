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
#include <cstdio>
#include <sys/_default_fcntl.h>
#include "miosix.h"
#include "FlashDriver.h"
#include "FileSystem.h"

using namespace std;
using namespace miosix;


void finalTest01(){
        iprintf("\nStarting the file System..Press a key to continue");
    char a;
    iscanf("%c", &a);
    iprintf("\n\n\n");
    FlashDriver& fd = FlashDriver::instance();
    FileSystem& fs=FileSystem::instance();
    if(a=='r'){
    iprintf("\nStart erasing...\n");
     if(fd.erase(8)==true)
         iprintf("\nErasing done with success\n");
     else{
         iprintf("\nErasing failed");
         return;
     }
    }
        memDump((char *) 0x08080000, 256);
        if(a=='a'){iprintf("\n\n Opening File01\n\n");
        File* File01=fs.open("File01",O_CREAT | O_WRONLY,8);
        iprintf("\n\n Opening File02\n\n");
                File* File02=fs.open("File02",O_CREAT | O_WRONLY,9);
        if(File01!=NULL && File02!=NULL){
            iprintf("\n\nFile01 and File02 opened with success!!! :-) \nStart writing on it!");
            File01->write("ciao!",5);
            File02->write("asd",3);
        }
                memDump((char *) 0x08080000, 256);
        }
        if(a=='u'){
            if(fs.unlink("File02")!=0)
                return;
            iprintf("unlinking done\n\n");
            
                          memDump((char *) 0x08080000, 256);
        }
        if(a=='b'){
            File* File03=fs.open("File03",O_CREAT | O_WRONLY,23);
            if(File03!=NULL)
                File03->write("123456789asdfghjkl",18);
            memDump((char *) 0x08080000, 256);
            iprintf("\n\n");
            char str[23];
            File03->read(str,23);
            iprintf("\n File03: %s",str);
        }
        if(a=='w'){
            File *File04=fs.open("nuovo",O_CREAT | O_WRONLY,8);
            if(File04==NULL){
                iprintf("\n\nERROR");
                return;
            }
            File04->write("visin",5);
            memDump((char *) 0x08080000, 256);
            }
    iprintf("\nTest done!");
    iprintf("\nTest done!");
}
Header readHeader(unsigned int address) {
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

void finalTest02(){
    FileSystem fs=FileSystem::instance();
    FlashDriver& fd = (FlashDriver::instance());

    iprintf("\nStarting the file System! \n");
    iprintf("Press:\nd to erase the FS\n");
    iprintf("s to view a sector\n");
    iprintf("o to open a new file\n");
    iprintf("u to unlink a file\n");
    iprintf("r to read a file\n");
    iprintf("l to view list of files\n");
    iprintf("i to view lstat of a file\n");
    char a;
    iscanf("%c", &a);
    if(a=='d'){
        iprintf("\nStarting erasing FS...");
        fs.clearFS();
        iprintf("\nErasing done!");
    }
    if(a=='s') {
        iprintf("Which sector do you want to see? (%d - %d)\n press 0 if you want to see an absolute address: ", 8, 11);
        int sector;
        iscanf("%d", &sector);
        if(sector==0){
            iprintf("\nPlease enter a absolute address in HEX");
            int add;
            iscanf("%x",&add);
            iprintf("how many bytes do you want to see starting from address %x?\n",add);
            int bytes;
            iscanf("%d",&bytes);
            iprintf("\nHere first %d bytes starting from address %x\n\n",bytes,add);
            memDump((char *) add, bytes);
            return;
        }
        iprintf("Do you want to see the whole sector? (y/n)\n");
        char b;
        iscanf("%c", &b);
        if (b == 'y') {
            iprintf("\nThere's sector number %d\n\n", sector);
            memDump((char *) (ADDR_FLASH_SECTOR_8 + (sector - 8) * SECTOR_SIZE), SECTOR_SIZE);
            return;
        }
        iprintf("how many bytes do you want to see starting from sector %d?\n",sector);
        int bytes=0;
        iscanf("%d",&bytes);
        iprintf("\nHere first %d bytes of sector number %d\n\n",bytes,sector);
        memDump((char *) (ADDR_FLASH_SECTOR_8 + (sector - 8) * SECTOR_SIZE), bytes);
        return;
    }
    if(a=='o') {
        iprintf("\nInsert name file (MAX LENGHT = %d) ",FS_FILE_NAME_MAX_LENGHT-1);
        char name[FS_FILE_NAME_MAX_LENGHT-1];
        iscanf("%s",name);
        iprintf("\nInsert file size in bytes:");
        int bytes=0;
        iscanf("%d",&bytes);
        iprintf("\nStarting opening file %s (size %d)\n",name,bytes);
        File *file=fs.open(name,O_CREAT | O_WRONLY,bytes);
        if(file==NULL){
            iprintf("Error opening file!\n");
            return;
        }
        char x;
        iprintf("\nFile %s opened with success.\n Do you want me to write something in it?",name);
        iscanf("%c",&x);
        x='y';
        char *ciao="ciao";
        if(x=='y'){
            for(int i=0;i<file->getSize();i++){
                file->write(&ciao[i%4],1);
            }
            return;
        }
        //TODO!!!!
        return;
    }
    if(a=='u'){
        iprintf("\nInsert the name of the file you want to unlink: ");
        char name[FS_FILE_NAME_MAX_LENGHT];
        iscanf("%s",name);
        if(fs.unlink(name)<0){
            iprintf("\nError unlinking the file");
            return;
        }
        iprintf("\nFile %s unlinked!",name);
        return;
    }
    if(a=='r'){
        iprintf("\nInsert the name of the file you want to read: ");
        char name[FS_FILE_NAME_MAX_LENGHT];
        iscanf("%s",name);
        iprintf("\nSearching the file in the FS");
        File *file=fs.open(name,O_RDONLY,0);
        if(file==NULL){
            iprintf("\nError opening file");
            return;
        }
        char buf;
        iprintf("\n%s:\n\n",name);
        for(int i=0;i<file->getSize();i++){
            file->read(&buf,1);
            iprintf("%c",buf);
        }
        iprintf("\n\n");
        
    }
    if(a=='l'){
        Header header;
        unsigned int address=fd.getStartSector();
        iprintf("\naddress name size\n");
        while(*(unsigned int *)address!=FS_BLANK){
            header=readHeader(address);
            iprintf("%x %s   %d\n",address,header.filename,header.next-address-sizeof(Header));
            address=header.next;
        }
        return;
    }
    
    if(a=='i'){
                iprintf("\nInsert the name of the file you want to view info: ");
        char name[FS_FILE_NAME_MAX_LENGHT];
        iscanf("%s",name);
        struct stat s;
        if(fs.lstat(name,&s)<0){
            iprintf("\nError unlinking the file");
            return;
        }
        iprintf("\nFile %s: \n",name);
        iprintf("size: %d\n",s.st_size);
        iprintf("blksize: %d\n",s.st_blksize);
        iprintf("blocks: %d\n",s.st_blocks);
        iprintf("uid: %d\n",s.st_uid);
        iprintf("gid: %d\n",s.st_gid);
        return;

    }
    

}

int main() {
    while(1){
    finalTest02();
    }
}
