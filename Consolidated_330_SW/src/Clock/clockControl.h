//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for controlling with the clock
//*****************************************************************************

#define MS_COUNTER_WAIT   49 // wait for 50, 20ms intervals
#define HALF_SECOND_WAIT  24  // wait for 25 20ms intervals
#define TENTH_SECOND_WAIT 4  // wait for 5 20ms intervals
#define ADC_WAIT          2   // wait for 50ms for ADC to settle

/**
 * This tick function controls the state transitions and state actions
 * of the clock control state machine.
 */
void clockControl_tick();
