/* add your code here */

#include <IOKit/IOService.h>
#include <libkern/OSKextLib.h>

#include "EchoUtility.h"


class EchoDriver : public IOService {
    OSDeclareDefaultStructors(EchoDriver);
public:
    bool start(IOService *provider);
    void stop(IOService *provider);
};