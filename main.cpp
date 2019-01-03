/* Includes -------------------------------------------- */
#ifndef __APPLE__
# include <asm/types.h>
# define DWORD  __u32
# define WORD   unsigned short
# define BYTE   unsigned char
# define LPSTR  char *
# include <PCANBasic.h>
#else
# include <PCBUSB.h>
#endif

#include <thread>
#include <chrono>
#include <iostream>

/* Variables ------------------------------------------- */
TPCANStatus lTPCANStatus;
/* Creating sync message */
TPCANMsg lSyncMsg = {
    0x80, 
    PCAN_MESSAGE_STANDARD, 
    0x0, 
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
};

/* Functions ------------------------------------------- */
bool sendCANSync(void) {
    lTPCANStatus = CAN_Write(PCAN_NONEBUS, &lSyncMsg);
    std::cout << "[INFO ] Sending sync message (COB ID 0x80)\n";

    if(PCAN_ERROR_OK == lTPCANStatus)
        return true; // For now
    else {
        std::cout << "[ERROR] Write failed w/ error code 0x" << std::hex << lTPCANStatus << std::endl;
        return false;
    }
}

/* Main ------------------------------------------------ */
int main(const int argc, const char * const * const argv){
    lTPCANStatus = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_1M);
    if(PCAN_ERROR_OK == lTPCANStatus)
        std::cout << "[DEBUG] Initialized CAN transmition for PCAN_NONEBUS @ PCAN_BAUD_1M : " << std::hex << lTPCANStatus << std::endl;
    else {
        std::cout << "[ERROR] Init failed w/ error code 0x" << std::hex << lTPCANStatus << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << "[DEBUG] Begining for(;;) loop\n";
    for(;;) {
        (void)sendCANSync();
        std::this_thread::sleep_for(std::chrono::seconds(1U));
    }
}