#include "File.h";
static FileSystem& fileSystem=FileSystem::instance();
static FlashDriver& flashDriver=FlashDriver::instance();
File::File(){
    
}
File& File::instance(){
    static File singleton;
    return singleton;
}
ssize_t File::write(int fd,const void *buff,size_t count){
    //TODO check the input!
    
    //here we assume that all the inputs are correct
    flashDriver.write(fd,(char *)buff,count);
}



