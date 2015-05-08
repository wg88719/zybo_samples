//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for controlling with the clock
//*****************************************************************************

#define MS_COUNTER_WAIT   100 // wait for 100, 10ms intervals
#define HALF_SECOND_WAIT  50  // wait for 50 10ms intervals
#define TENTH_SECOND_WAIT 10  // wait for 10 10ms intervals
#define ADC_WAIT          5   // wait for 50ms for ADC to settle

/**
 * This tick function controls the state transitions and state actions
 * of the clock control state machine.
 */
void clockControl_tick();
