
#include <cstdio>
#include <sys/_default_fcntl.h>
#include "miosix.h"
#include "FlashDriver.h"
#include "FileSystem.h"

#define DATA32 ((unsigned int)1991)
using namespace std;
using namespace miosix;

void testFlashDriver01() {
    FlashDriver& fd = (FlashDriver::instance());
    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
    char a;
    iscanf("%c", &a);
    iprintf("\nStart erasing...\n");
    if (fd.erase(7) == true)
        iprintf("\nErasing done with success\n");
    else {
        iprintf("\nErasing failed");
        return;
    }
    iprintf("\nStart writing...\n");
    if (fd.write(ADDR_FLASH_SECTOR_7, DATA32) == true)
        iprintf("\nWriting done with success\n");
    else {
        iprintf("\nWriting failed");
        return;
    }
    int data32 = 0;
    iprintf("\nStart reading...\n");
    data32 = *(int*) ADDR_FLASH_SECTOR_7;
    iprintf("\nReading done\n");

    if (data32 == DATA32) {
        iprintf("\nWORKS!!!!  :-)");
        iprintf("\n%d", data32);
    } else {
        iprintf("\nNOPE!!! :-(");
        iprintf("%d", data32);
        iprintf("Instead of %d", DATA32);
    }
    iprintf("\nFinished!");
    iprintf("\nFinished!");

}

void testFlashDriver02() {
    FlashDriver& fd = (FlashDriver::instance());
    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
    char a;
    iscanf("%c", &a);
    /* iprintf("\nStart erasing...\n");
     if(fd.erase(7)==true)
         iprintf("\nErasing done with success\n");
     else{
         iprintf("\nErasing failed");
         return;
     }*/
    unsigned int address = ADDR_FLASH_SECTOR_7;
    while (address < ADDR_FLASH_SECTOR_8) {
        if (fd.write(address, 0xAAAAAAAA) == false) {
            iprintf("\nERROR writing");
            break;
        }
        address += 4;
    }
    iprintf("\nwriting done...");
    memDump((char *) 0x08060000, 1000);

}

/**
 * Test the casting:
 * try to write a double, int, short and char.
 */
void testFlashDriver03() {
    FlashDriver& fd = (FlashDriver::instance());
    iprintf("\nStarting writing in to the flash...\n Press a key to continue...");
    char a;
    iscanf("%c", &a);
    /*iprintf("\nStart erasing...\n");
    if(fd.erase(8)==true)
        iprintf("\nErasing done with success\n");
    else{
        iprintf("\nErasing failed");
        return;
    }*/
    unsigned int address = 0x08080015;
    address += sizeof (char);
    iprintf("\nWriting a short %x in %x", (short) 0xA2A3, address);
    if (fd.write(address, (short) 0xA2A3) == false) {
        iprintf("\nWriting failed");
        return;
    }
    iprintf("I just wrote %x", *(short *) address);
    address += sizeof (short);
    iprintf("\nWriting a int in %x", address);
    if (fd.write(address, address) == false) {
        iprintf("\nWriting failed");
        return;
    }
    iprintf("I just wrote %x", address);
    address += sizeof (unsigned int);
    memDump((char *) 0x08080000, 1000);
}


void testFileSystemOpen01() {
    iprintf("\nStarting the file System..Press a key to continue");
    char a;
    iscanf("%c", &a);
    FlashDriver& fd = FlashDriver::instance();
    FileSystem& fs = FileSystem::instance();
    /*iprintf("\nStart erasing...\n");
     if(fd.erase(8)==true)
         iprintf("\nErasing done with success\n");
     else{
         iprintf("\nErasing failed");
         return;
     }*/
    iprintf("\nFileSystem started");
    int ret = fs.open("ciaociao123.jpg", O_CREAT, 26);
    iprintf("\nopened file ciaociao123.jpg with returning value:%x\n ", ret);
    /*ret=fs.open("aspodk123.jpg",O_CREAT,26);
    iprintf("\nopened file aspodk123.jpg with returning value:%x\n ",ret);
    ret=fs.open("Prova02.TXT",O_CREAT,5);
    iprintf("\nopened file PROVA02.txt with returning value:%x\n ",ret);
    ret=fs.open("Prova03.TXT",O_CREAT,5);
    iprintf("\nopened file PROVA03.txt with returning value:%x\n ",ret);*/

    memDump((char *) 0x08080000, 1000);
}

void testFileSystem03() {
    iprintf("\nStarting the file System..Press a key to continue");
    char a;
    iscanf("%c", &a);
    FileSystem& fs = FileSystem::instance();
    int r = fs.unlink("ciaociao123.jpg");
    iprintf("\n\nciaociao123.jpg Valore Ritorno:  %x    %d", r, r);

    r = fs.unlink("Prova01.TXT");
    iprintf("\n\nProva01.TXT Valore Ritorno:  %x    %d", r, r);

    r = fs.unlink("ciaociao.jpg");
    iprintf("\n\nciaociao.jpg Valore Ritorno:  %x    %d", r, r);


    r = fs.unlink("Prova02.TXT");
    iprintf("\n\nProva02.TXT Valore Ritorno:  %x    %d", r, r);

    r = fs.unlink("FileInesistente");
    iprintf("\n\nFileInesistente Valore Ritorno:  %x    %d", r, r);


    memDump((char *) 0x08080000, 256);


}

void testDelete(){
    iprintf("\nStarting the file System..Press a key to continue");
    char a;
    iscanf("%c", &a);
    FileSystem& fs = FileSystem::instance();
    int r = fs.unlink("Prova02.TXT");
    iprintf("\n\nProva02.TXT Valore Ritorno:  %x    %d\n", r, r);
    memDump((char *) 0x08080000, 256);

}

int main() {
    //testDelete();
    File& file=File::instance();
    FileSystem fs=FileSystem::instance();
    iprintf("\nTest to write a file into the FS!!");
    char a;
    iscanf("%c", &a);
    
    int fd=fs.open("FILE02.txt",O_CREAT,16);
    file.write(fd,"Tutto bene dai!!",16);
    
    memDump((char *) 0x08080000, 512);

    iprintf("\nTest done!");
    iprintf("\nTest done!");
}
