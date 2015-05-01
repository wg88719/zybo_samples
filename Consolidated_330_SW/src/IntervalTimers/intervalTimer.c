//*****************************************************************************
// Luke Hsiao
// 1 May 2015
// Implementation of the AXI Interval Timer Interface
//*****************************************************************************

#include <stdio.h>
#include <xil_io.h>
#include "xparameters.h"
#include "intervalTimer.h"

// Helper function for reading timer registers
// @param timerNumber {0,1,2}
// @param offset Offset from the base address
// @return the 32-bit register value or TIMER_ERROR for invalid numbers
uint32_t intervalTimer_readTimerRegister(uint32_t timerNumber, uint8_t offset) {
  u32 regValue;
  switch (timerNumber) {
    case 0:
      regValue = Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + offset);
      break;
    case 1:
      regValue = Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + offset);
      break;
    case 2:
      regValue = Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + offset);
      break;
    default:  // invalid timer number
      printf("\nERROR: Not a valid timer number.\n\n");
      regValue = TIMER_ERROR;
      break;
  }
  //cast regValue value to an uint32_t and return
  return (uint32_t)regValue;
}

// Helper function for writing timer registers
// @param timerNumber {0,1,2}
// @param offset Offset from the base address
// @param value value to write to the register
// @return 0 if successful TIMER_ERROR for invalid numbers
uint32_t intervalTimer_writeTimerRegister(uint32_t timerNumber, uint8_t offset, uint32_t value) {
  
  uint32_t status = 0;
  switch (timerNumber) {
    case 0:
      Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + offset, value);
      break;
    case 1:
      Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + offset, value);
      break;
    case 2:
      Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + offset, value);
      break;
    default:  // invalid timer number
      printf("\nERROR: Not a valid timer number.\n\n");
      status = TIMER_ERROR;
      break;
  }
  // return 0 for success, TIMER_ERROR for invalid timer number
  return status;
}

// Helper function for writing timer registers
// @param timerNumber {0,1,2}
// @return frequency if successful or TIMER_ERROR for invalid numbers
uint32_t intervalTimer_getTimerFrequency(uint32_t timerNumber) {
  uint32_t frequency;
  switch (timerNumber) {
    case 0:
      frequency = XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
      break;
    case 1:
      frequency = XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ;
      break;
    case 2:
      frequency = XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ;
      break;
    default:  // invalid timer number
      printf("\nERROR: Not a valid timer number.\n\n");
      frequency = TIMER_ERROR;
      break;
  }
  // return 0 for success, TIMER_ERROR for invalid timer number
  return frequency;
}

// Helper function that enables the ENT0 bit of the passed-in value
uint32_t intervalTimer_enableENT0(uint32_t original) {
  return (original | ENABLE_ENT0_MASK);
}

// Helper function that clears the ENT0 bit of the passed-in value
uint32_t intervalTimer_clearENT0(uint32_t original) {
  return (original & CLEAR_ENT0_MASK);
}

// Helper function that enables the CASC bit of the passed-in value
uint32_t intervalTimer_enableCASC(uint32_t original) {
  return (original | ENABLE_CASC_MASK);
}

// Helper function that enables the LOAD0 bit of the passed-in value
uint32_t intervalTimer_enableLOAD0(uint32_t original) {
  return (original | ENABLE_LOAD0_MASK);
}

uint32_t intervalTimer_start(uint32_t timerNumber) {
  uint32_t csrValue;
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }
  
  // Get the current value of control/status register
  csrValue = intervalTimer_readTimerRegister(timerNumber, TCSR0_OFFSET);
  
  uint32_t enableENT0 = intervalTimer_enableENT0(csrValue);

  // Write value back w/ ENT0 enabled
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR0_OFFSET, // register offset
                                    enableENT0);
  
  return 0;  // return 0 for success, or TIMER_ERROR
}

uint32_t intervalTimer_stop(uint32_t timerNumber) {
  uint32_t csrValue;
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }
  
  // Get the current value of control/status register
  csrValue = intervalTimer_readTimerRegister(timerNumber, TCSR0_OFFSET);
  
  // Write value back w/ ENT0 enabled
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR0_OFFSET, // register offset
                                    intervalTimer_clearENT0(csrValue));
  
  return 0;  // return 0 for success, or TIMER_ERROR
}

uint32_t intervalTimer_reset(uint32_t timerNumber) {

  uint32_t csrValue;
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }
  
  // Store 0 in TLR0
  intervalTimer_writeTimerRegister(timerNumber, TLR0_OFFSET, 0x0000);
  
  // Get the current value of control/status register
  csrValue = intervalTimer_readTimerRegister(timerNumber, TCSR0_OFFSET);
  
  // Write value back w/ LOAD0 enabled
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR0_OFFSET, // register offset
                                    intervalTimer_enableLOAD0(csrValue));
                                    
  // Store 0 in TLR1
  intervalTimer_writeTimerRegister(timerNumber, TLR1_OFFSET, 0x0000);
  
  // Get the current value of control/status register
  csrValue = intervalTimer_readTimerRegister(timerNumber, TCSR1_OFFSET);
  
  // Write value back w/ LOAD0 enabled
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR1_OFFSET, // register offset
                                    intervalTimer_enableLOAD0(csrValue));
  
  // Set all CSR bits back to 0 except for CASC
  intervalTimer_init(timerNumber);

  return 0;  // return 0 for success, or TIMER_ERROR
}

uint32_t intervalTimer_init(uint32_t timerNumber) {
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }
  
  // Write 0 to TCSR0
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR0_OFFSET, // register offset
                                    0x0000);
  
  // Write 0 to TCSR1
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR1_OFFSET, // register offset
                                    0x0000);
                                    
  // Set cascade bit. Note that since TCSR was set to zero, timer counts up
  intervalTimer_writeTimerRegister( timerNumber, // timer number
                                    TCSR0_OFFSET, // register offset
                                    intervalTimer_enableCASC(0x0000));
  

  return 0;  // return 0 for success, or TIMER_ERROR
}

uint32_t intervalTimer_initAll() {
  uint32_t status;
  int i;
  for (i = 0; i <= TIMER2; i++) {
    status = intervalTimer_init(i);
    
    // If an error occurred, break and return TIMER_ERROR
    if (status != 0) {
      return status;
    }
  }
  
  return 0; // return 0 on success, TIMER_ERROR on failure
}

uint32_t intervalTimer_resetAll() {
  uint32_t status;
  int i;
  for (i = 0; i <= TIMER2; i++) {
    status = intervalTimer_reset(i);
    
    // If an error occurred, break and return TIMER_ERROR
    if (status != 0) {
      return status;
    }
  }
  
  return 0; // return 0 on success, TIMER_ERROR on failure
}

uint32_t intervalTimer_testAll() {
  uint32_t status;
  int i;
  for (i = 0; i <= TIMER2; i++) {
    status = intervalTimer_runTest(i);
    
    // If an error occurred, break and return TIMER_ERROR
    if (status != 0) {
      return status;
    }
  }
  
  return 0; // return 0 on success, TIMER_ERROR on failure
}



uint32_t intervalTimer_runTest(uint32_t timerNumber) {
  
  uint32_t status = 0;
  double seconds = 0;
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }

  // reset the timer
  printf("Resetting timer %d\n", (int) timerNumber);
  status = intervalTimer_reset(timerNumber);
  
  // Check that timer is reset
  printf("Counter value should be ZERO:\n\t");
  status = intervalTimer_getTotalDurationInSeconds(timerNumber, &seconds);
  printf("Timer %d: Counter Value = %f\n", (int) timerNumber, seconds);
  
  // Start the Timer and show that it is running
  printf("Starting the timer...\n");
  status = intervalTimer_start(timerNumber);
  
  printf("Timer value should be changing:\n\t");
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  
  // Stop the timer
  printf("Stopping the timer...\n");
  status = intervalTimer_stop(timerNumber);
  
  printf("Timer value should NOT be changing:\n\t");
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n\t", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));
  printf("TCR0:%ld\n", intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET));

  
  if (status != 0) {
    printf("\n>>>> AN ERROR OCCURED <<<<\n\n");
  }
  return status;
}

uint32_t intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber, double *seconds) {
  uint64_t tempSeconds = 0;
  uint32_t lower_bits, upper_bits, upper_bits_check;
  
  // Check for invalid timer numbers
  if (timerNumber > TIMER2) {
    // Invalid timerNumber was passed in!
    printf("\nERROR: Not a valid timer number.\n\n");
    return TIMER_ERROR;
  }
  
  // Read upper 32-bits of counter
  upper_bits = intervalTimer_readTimerRegister(timerNumber, TCR1_OFFSET);
  // Read lower 32-bits of counter
  lower_bits = intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET);
  
  // Read upper bits again
  upper_bits_check = intervalTimer_readTimerRegister(timerNumber, TCR1_OFFSET);
  
  // if they differ, read lower bits again
  if (upper_bits_check != upper_bits) {
    lower_bits = intervalTimer_readTimerRegister(timerNumber, TCR0_OFFSET);
  }
  
  tempSeconds = upper_bits;
  tempSeconds = tempSeconds << REGISTER_WIDTH;  // shift upper bits up
  tempSeconds = tempSeconds + lower_bits;
  
  // Set the seconds value
  *seconds = tempSeconds / intervalTimer_getTimerFrequency(timerNumber);
  
  return 0;
}
