
#include <cstdio>
#include <sys/_default_fcntl.h>
#include "miosix.h"
#include "FlashDriver.h"
#include "FileSystem.h"
#include "kernel/filesystem/integer.h"

#define DATA32 ((unsigned int)1991)
using namespace std;
using namespace miosix;
//
//void testFlashDriver01() {
//    FlashDriver& fd = (FlashDriver::instance());
//    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
//    char a;
//    iscanf("%c", &a);
//    iprintf("\nStart erasing...\n");
//    if (fd.erase(7) == true)
//        iprintf("\nErasing done with success\n");
//    else {
//        iprintf("\nErasing failed");
//        return;
//    }
//    iprintf("\nStart writing...\n");
//    if (fd.write(ADDR_FLASH_SECTOR_7, DATA32) == true)
//        iprintf("\nWriting done with success\n");
//    else {
//        iprintf("\nWriting failed");
//        return;
//    }
//    int data32 = 0;
//    iprintf("\nStart reading...\n");
//    data32 = *(int*) ADDR_FLASH_SECTOR_7;
//    iprintf("\nReading done\n");
//
//    if (data32 == DATA32) {
//        iprintf("\nWORKS!!!!  :-)");
//        iprintf("\n%d", data32);
//    } else {
//        iprintf("\nNOPE!!! :-(");
//        iprintf("%d", data32);
//        iprintf("Instead of %d", DATA32);
//    }
//    iprintf("\nFinished!");
//    iprintf("\nFinished!");
//
//}
//
//void testFlashDriver02() {
//    FlashDriver& fd = (FlashDriver::instance());
//    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
//    char a;
//    iscanf("%c", &a);
//    /* iprintf("\nStart erasing...\n");
//     if(fd.erase(7)==true)
//         iprintf("\nErasing done with success\n");
//     else{
//         iprintf("\nErasing failed");
//         return;
//     }*/
//    unsigned int address = ADDR_FLASH_SECTOR_7;
//    while (address < ADDR_FLASH_SECTOR_8) {
//        if (fd.write(address, 0xAAAAAAAA) == false) {
//            iprintf("\nERROR writing");
//            break;
//        }
//        address += 4;
//    }
//    iprintf("\nwriting done...");
//    memDump((char *) 0x08060000, 1000);
//
//}
//
///**
// * Test the casting:
// * try to write a double, int, short and char.
// */
//void testFlashDriver03() {
//    FlashDriver& fd = (FlashDriver::instance());
//    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
//    char a;
//    iscanf("%c", &a);
//    /*iprintf("\nStart erasing...\n");
//    if(fd.erase(8)==true)
//        iprintf("\nErasing done with success\n");
//    else{
//        iprintf("\nErasing failed");
//        return;
//    }*/
//    unsigned int address = 0x08080015;
//    address += sizeof (char);
//    iprintf("\nWriting a short %x in %x", (short) 0xA2A3, address);
//    if (fd.write(address, (short) 0xA2A3) == false) {
//        iprintf("\nWriting failed");
//        return;
//    }
//    iprintf("I just wrote %x", *(short *) address);
//    address += sizeof (short);
//    iprintf("\nWriting a int in %x", address);
//    if (fd.write(address, address) == false) {
//        iprintf("\nWriting failed");
//        return;
//    }
//    iprintf("I just wrote %x", address);
//    address += sizeof (unsigned int);
//    memDump((char *) 0x08080000, 1000);
//}
//
///
//void testFileSystemOpen01() {
//    iprintf("\nStarting the file System..Press a key to continue");
//    char a;
//    iscanf("%c", &a);
//    FlashDriver& fd = FlashDriver::instance();
//    FileSystem& fs = FileSystem::instance();
//    iprintf("\nStart erasing...\n");
//     if(fd.erase(8)==true)
//         iprintf("\nErasing done with success\n");
//     else{
//         iprintf("\nErasing failed");
//         return;
//     }
//    iprintf("\nFileSystem started");
//    int ret = fs.open("ciaociao123.jpg", O_CREAT, 26);
//    iprintf("\nopened file ciaociao123.jpg with returning value:%x\n ", ret);
//    /*ret=fs.open("aspodk123.jpg",O_CREAT,26);
//    iprintf("\nopened file aspodk123.jpg with returning value:%x\n ",ret);
//    ret=fs.open("Prova02.TXT",O_CREAT,5);
//    iprintf("\nopened file PROVA02.txt with returning value:%x\n ",ret);
//    ret=fs.open("Prova03.TXT",O_CREAT,5);
//    iprintf("\nopened file PROVA03.txt with returning value:%x\n ",ret);*/
//
//    memDump((char *) 0x08080000, 1000);
//}
//
//void testFileSystem03() {
//    iprintf("\nStarting the file System..Press a key to continue");
//    char a;
//    iscanf("%c", &a);
//    FileSystem& fs = FileSystem::instance();
// 
//    int r = fs.unlink("ciaociao123.jpg");
//    iprintf("\n\nciaociao123.jpg Valore Ritorno:  %x    %d", r, r);
//
//    r = fs.unlink("Prova01.TXT");
//    iprintf("\n\nProva01.TXT Valore Ritorno:  %x    %d", r, r);
//
//    r = fs.unlink("ciaociao.jpg");
//    iprintf("\n\nciaociao.jpg Valore Ritorno:  %x    %d", r, r);
//
//
//    r = fs.unlink("Prova02.TXT");
//    iprintf("\n\nProva02.TXT Valore Ritorno:  %x    %d", r, r);
//
//    r = fs.unlink("FileInesistente");
//    iprintf("\n\nFileInesistente Valore Ritorno:  %x    %d", r, r);
//
//
//    memDump((char *) 0x08080000, 256);
//
//
//}
//
//void testDelete(){
//    iprintf("\nStarting the file System..Press a key to continue");
//    char a;
//    iscanf("%c", &a);
//    FileSystem& fs = FileSystem::instance();
//    int r = fs.unlink("Prova02.TXT");
//    iprintf("\n\nProva02.TXT Valore Ritorno:  %x    %d\n", r, r);
//    memDump((char *) 0x08080000, 256);
//
//}
/*
void testFile01(){
    iprintf("\nStarting the file System..Press a key to continue");
    char a;
    iscanf("%c", &a);
    iprintf("\n\n\n");
    FileSystem fs=FileSystem::instance();
           FlashDriver& fd = FlashDriver::instance();
           if(a=='r'){
    iprintf("\nStart erasing...\n");
     if(fd.erase(8)==true)
         iprintf("\nErasing done with success\n");
     else{
         iprintf("\nErasing failed");
         return;
     }
           }
    if(a=='!'){
    int fd=fs.open("File01",O_CREAT | O_WRONLY,80);
    File* file=new File(fd,80);
    file->write(fd,"Contenuto",9);
    
    char r[30];
    file->read(fd,r,10);
    iprintf("\n\n In File01: \n%s\n",r);
    }
    if(a=='"'){
        int fd=fs.open("File02",O_CREAT | O_WRONLY,80);
    File* file=new File(fd,80);
    file->write(fd,"Ciao, questo è il mio file",27);
    
    char r[30];
    file->read(fd,r,30);
    iprintf("\n\n In File03: \n%s\n",r);
    }
    if(a=='p'){
        int fd=fs.open("File03",O_CREAT | O_WRONLY,80);
    File* file=new File(fd,80);
    file->write(fd,"CIAOCIAO",8);
    char r[10];
    file->read(fd,r,10);
    iprintf("\n\n In File03: \n%s\n",r);
    }
    iprintf("\n\n Before unlinking File02\n\n");
    memDump((char *) 0x08080000, 1024);
    int ret=-1;
    if(a=='2')
        ret=fs.unlink("File02");
    else if(a=='1')
        ret=fs.unlink("File01");
    else if(a=='3')
        ret=fs.unlink("File03");
    iprintf("\n\nret addr:%x\n\n",ret);
    iprintf("\n\n After unlinking File   %c\n\n",a);
    memDump((char *) 0x08080000, 1024); 
}
*/

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
    iprintf("l to view list of file\n");
    char a;
    iscanf("%c", &a);
    if(a=='d'){
        iprintf("\nStarting erasing FS...");
        fs.clearFS();
        iprintf("\nErasing done!");
    }
    if(a=='s') {
        iprintf("Which sector do you want to see? (%d - %d)", 8, 11);
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
        if(x=='y'){
            for(int i=0;i<file->getSize();i+=4){
                file->write("ciao",4);
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
    

}

int main() {
    while(1){
    finalTest02();
    }
}
