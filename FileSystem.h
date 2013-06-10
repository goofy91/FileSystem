/* 
 * File:   FileSystem.h
 * Author: goofy
 *
 * Created on June 3, 2013, 2:41 PM
 */
#include "File.h"
#include <errno.h>
#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#define ALIGMENT 4//in byte
//ATTENTION size of the filename length field must be divisible for 2
#define FS_FILE_NAME_MAX_LENGHT 24//6*ALIGMENT in bytes
#define FS_ADDR_ROOT ADDR_FLASH_SECTOR_8
#define FS_ADDR_ROOF ADDR_FLASH_SECTOR_10
#define FS_ADDR_BUFFER ADDR_FLASH_SECTOR_11

#define FS_SECTOR_ROOT 4
#define FS_SECTOR_ROOF 10
#define FS_SECTOR_BUFFER 11
#define FS_ROOT_DIM ALIGMENT
#define FS_BLANK (unsigned int)0xFFFFFFFF
//HEADER STRUCT!
struct Header{
    unsigned int next;
    unsigned int size;
    unsigned int flag1;
    unsigned int flag2;
    char filename[FS_FILE_NAME_MAX_LENGHT];
};
class FileSystem{
public:
    /**
     * open a new file in the FS
     * @param filename
     * @param flags
     * @param size
     * @return new file descriptor or -1 if an error occurred
     */
    int open(const char *filename,int flags,int size);
    int lstat(char *filename,struct stat* buf);
    int unlink(const char *filename);
    
    /**
     * clear all the FS erasing all the sectors of the FS
     */
    static void clearFS();
    /**
     * 
     * @return the instance of the FileSystem 
     */
    static FileSystem& instance();

private:


    FileSystem();
    /**
     * Tested!!
     * @param address where the header is
     * @return the header
     */
    static struct Header readHeader(unsigned int address);
    /**
     * This function finds a file in the FS starting from the root.
     * The root must exist to run this function.
     * @return true if the file exist, otherwise false 
     */
    bool findFile(const char *filename);

    /**
     * 
     * @param address where write the header
     * @param header
     * @return the new address
     */
    unsigned int writeHeader(unsigned int address,Header header);
    /**
     * 
     * @param filename to find
     * @return The address of the filename, FS_BLANK if the file doesn't exist
     */
    unsigned int getAddress(const char* filename);
    /**
     * 
     * @param filename to find
     * @return The address of the file before the input filename, FS_BLANK if the file doesn't exist
     */
    unsigned int getAddressBefore(const char* filename);
    
    /**
     * Check if from the address "from" to the address "to" the bits are blank
     * @param from address
     * @param to address
     * @return true if the whole space is blank, otherwise false
     */
    bool checkBlank(unsigned int from,unsigned int to);


    
};


#endif	/* FILESYSTEM_H */

