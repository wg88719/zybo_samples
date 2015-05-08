//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for controlling with the clock
//*****************************************************************************

#define SECOND_WAIT       20  // wait for 20 50ms intervals
#define HALF_SECOND_WAIT  10  // wait for 10 50ms intervals
#define TENTH_SECOND_WAIT 2   // wait for 2 50ms intervals
#define ADC_WAIT          1   // wait for 50ms for ADC to settle

/**
 * This tick function controls the state transitions and state actions
 * of the clock control state machine.
 */
void clockControl_tick();
