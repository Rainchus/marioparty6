#include <dolphin.h>

// EXI error codes
typedef enum { AMC_EXI_NO_ERROR = 0, AMC_EXI_UNSELECTED } AmcExiError;

int AMC_IsStub();
void EXI2_Unreserve();
void EXI2_Reserve();
AmcExiError EXI2_WriteN();
AmcExiError EXI2_ReadN();
int EXI2_Poll();
void EXI2_EnableInterrupts();
void EXI2_Init();

void EXI2_Init() { }

void EXI2_EnableInterrupts() { }

int EXI2_Poll()
{
    return 0;
}

AmcExiError EXI2_ReadN()
{
    return AMC_EXI_NO_ERROR;
}

AmcExiError EXI2_WriteN()
{
    return AMC_EXI_NO_ERROR;
}

void EXI2_Reserve() { }

void EXI2_Unreserve() { }

int AMC_IsStub()
{
    return 1;
}
