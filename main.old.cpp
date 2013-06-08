
#include <cstdio>
#include "miosix.h"

using namespace std;
using namespace miosix;

int main()
{
    printf("Hello world, write your application here\n");
}

class FlashDriver
{
public:
    FlashDriver& instance();
    /**
     * 
     * @param address
     * @return true on success, false on failure
     */
    bool erase(int sector);
    /**
     * Erase all the sectors
     * @return true on success, false on failure
     */
    bool erase();
   
    bool wirte(int address,int data);
private:
    FlashDriver();
    bool UnlockFlashCR();
};

FlashDriver& FlashDriver::instance()
{
    static FlashDriver singleton;
    return singleton;
}

FlashDriver::FlashDriver()
{
    
}
/* Fatta dal prof
bool FlashDriver::erase(int sector)
{
    InterruptDisableLock dLock;
    FLASH->OPTKEYR = 0x08192A3B;
    FLASH->OPTKEYR = 0x4C5D6E7F;
    if(FLASH->CR & FLASH_CR_LOCK) return false;
    FLASH->CR = sector<<3 | FLASH_CR_SER | FLASH_CR_STRT;
    while(FLASH->SR & FLASH_SR_BSY) ;
    FLASH->CR |= FLASH_CR_LOCK;
    return true;
}
*/
bool FlashDriver::UnlockFlashCR(){
    FLASH->OPTKEYR = 0x08192A3B;
    FLASH->OPTKEYR = 0x4C5D6E7F;
    if(FLASH->CR & FLASH_CR_LOCK) return false;
    return true;
}
bool FlashDriver::erase(int sector)
{
    InterruptDisableLock dLock;
    if(FLASH->SR & FLASH_SR_BSY) return false;
    if((sector & 0xFFF0)==0) return false;// wrong sector
    if(!this->UnlockFlashCR()) return false;
    FLASH->CR |= sector<<3 | FLASH_CR_SER | FLASH_CR_STRT;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR |=FLASH_CR_LOCK;
    return true;
}
bool FlashDriver::erase()
{
    InterruptDisableLock dLock;
    if(FLASH->SR & FLASH_SR_BSY) return false;
    if(!this->UnlockFlashCR()) return false;
    FLASH->CR |= FLASH_CR_MER | FLASH_CR_STRT;
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR |=FLASH_CR_LOCK;
    return true;
}

bool FlashDriver::wirte(int address, int data)
{
    InterruptDisableLock dLock;
    if(FLASH->SR & FLASH_SR_BSY) return false;
    if(!this->UnlockFlashCR()) return false;
    FLASH->CR |= FLASH_CR_PG;
    //write:
    
    //
    while(FLASH->SR & FLASH_SR_BSY);
    FLASH->CR |=FLASH_CR_LOCK;
    return true;
}