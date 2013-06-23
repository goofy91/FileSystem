/* 
 * File:   FileSystem.h
 * Author: goofy
 *
 * Created on June 3, 2013, 2:41 PM
 */
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>

#include "FlashDriver.h"
#include "File.h"
#ifndef FILESYSTEM_H
#define	FILESYSTEM_H

#define ALIGMENT 4//in byte
//ATTENTION size of the filename length field must be divisible for 2
#define FS_FILE_NAME_MAX_LENGHT 24//6*ALIGMENT in bytes

#define FS_SECTOR_ROOT 4
#define FS_SECTOR_ROOF 10
#define FS_SECTOR_BUFFER 11
#define FS_ROOT_DIM ALIGMENT
#define FS_BLANK (unsigned int)0xFFFFFFFF

//HEADER STRUCT!
struct Header{
    unsigned int next;
    unsigned int size;
    uid_t uid;
    gid_t gid;
    char filename[FS_FILE_NAME_MAX_LENGHT];
};
class File{
public:
    ssize_t write(const void *buff,size_t count);
    ssize_t read(void *buf,size_t count);
    int fstat(struct stat *buf);
    off_t lseek(off_t offset,int whence);
    unsigned int getBasePointer();
    File(unsigned int basePointer,size_t lengh);

private:
    unsigned int basePointer;
    int fp;
    size_t lenght;
    
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
    File* open(const char *filename,int flags,int size);
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
     * @return true on success false on failure
     */
    bool writeHeader(unsigned int address,Header header);
    /**
     * 
     * @param filename to find
     * @return The address of the filename, FS_BLANK if the file doesn't exist
     */
    unsigned int getAddress(const char* filename);
    
    /**
     * Check if from the address "from" to the address "to" the bits are blank
     * @param from address
     * @param to address
     * @return true if the whole space is blank, otherwise false
     */
    bool checkBlank(unsigned int from,unsigned int to);

    /**
     * 
     * @param address of the file
     * @return true if the file is valid, otherwise false.
     */
    bool isValid(unsigned int address);

    uid_t getuid(){return 0xBBBB;}
    
    gid_t getgid(){return 0xAAAA;}
};
#endif	/* FILESYSTEM_H */

