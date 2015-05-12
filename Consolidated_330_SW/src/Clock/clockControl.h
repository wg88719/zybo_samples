//*****************************************************************************
// Luke Hsiao
// 4 May 2015
// Interface for controlling with the clock
//*****************************************************************************

// Constants for controlling how long actions wait
#define CLOCKCONTROL_SECOND_WAIT       20  // wait for 20 50ms intervals
#define CLOCKCONTROL_HALF_SECOND_WAIT  10  // wait for 10 50ms intervals
#define CLOCKCONTROL_TENTH_SECOND_WAIT 2   // wait for 2 50ms intervals
#define CLOCKCONTROL_ADC_WAIT          1   // wait for 50ms for ADC to settle

/**
 * This tick function controls the state transitions and state actions
 * of the clock control state machine.
 */
void clockControl_tick();
