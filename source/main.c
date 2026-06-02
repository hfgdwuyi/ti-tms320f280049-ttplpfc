//
// Included Files
//
#include "device.h"
#include "Calculare.h"
#include "background_task_cla.h"
//
// Globals
//
uint32_t cpuTimer0IntCount;
uint32_t cpuTimer1IntCount;
uint32_t cpuTimer2IntCount;
uint32_t epwmIntCount;
uint16_t txMsgData[8];
uint16_t rxMsgData[8];
unsigned char *msg;

//
// Function Prototypes
//
__interrupt void cpuTimer0ISR(void);
//__interrupt void cpuTimer1ISR(void);
//__interrupt void cpuTimer2ISR(void);
__interrupt void canISR(void);
__interrupt void sciaRxISR(void);
__interrupt void epwm1ISR(void);
__interrupt void gpioISR(void);
//__interrupt void adcISR(void);

void initCPUTimers(void);
void InitDSP_Timer(void);
void configCPUTimer(uint32_t, float, float);
void InitDSP_Can(void);
void InitDSP_Device(void);
void InitDSP_Gpio(void);
void InitDSP_Epwm(void);
void InitDSP_Adc(void);
void InitDSP_Scia(void);
void InitDSP_Interrupt(void);
void InitVar_Data(void);
void InitDSP_Dac(void);
void InitDSP_Cla(void);
void InitDSP_MemCfg(void);

//
// Defines
//
#define MSG_DATA_LENGTH     8
#define TX_MSG_OBJ_ID1      1
#define TX_MSG_OBJ_ID2      2
#define TX_MSG_OBJ_ID3      3
#define TX_MSG_OBJ_ID4      4
#define TX_MSG_OBJ_ID5      5
#define RX_MSG_OBJ_ID       10

//
// Main
//
void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Initialize the DSP device
    //
    InitDSP_Device();

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

    //
    // Initialize the various data for application
    //
    InitVar_Data();

    //
    // Loop Forever
    //
    while(1)
    {
        DAC_setShadowValue(DACB_BASE, 4000);
        DEVICE_DELAY_US(2);
        TimeBase_Manage();
        GridPeriodRmsCal();
        State_Manage();
//        WarnCtrl_Manage();
    }
}

//
// initialize all kinds of the various
//
void InitVar_Data(void)
{
    TimeBase_Var_Init();
    State_Var_Init();
    VoltLoop_Var_Init();
    CurrLoop_Var_Init();
    PWMDriver_Var_Init();
    WarnCtrl_Var_Init();
}

void InitDSP_Device(void)
{
    //
    // Configure interrupt module
    //
    InitDSP_Interrupt();

    //
    // Configure timer module
    //
    InitDSP_Timer();

    //
    // Configure gpio module
    //
    InitDSP_Gpio();

    //
    // Configure pwm module
    //
    InitDSP_Epwm();

    //
    // Configure adc module
    //
    InitDSP_Adc();

    //
    // Configure dac module
    //
    InitDSP_Dac();

    //
    // Configure uart module
    //
    InitDSP_Scia();

    //
    // Configure can module
    //
    InitDSP_Can();

    //
    // Configure cla module
    //
    InitDSP_Cla();
    InitDSP_MemCfg();

}


void InitDSP_Timer(void)
{
    //
    // Initialize the Device Peripheral. For this example, only initialize the
    // Cpu Timers.
    //
    initCPUTimers();

    //
    // Configure CPU-Timer 0, 1, and 2 to interrupt every second:
    // 100MHz CPU Freq, 1 second Period (in uSeconds)
    //
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 1000);
//    configCPUTimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, 100);
//    configCPUTimer(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ, 1000);

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
//    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
//    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    //
    // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
    // CPU-Timer 1, and CPU-Timer 2 respectively.
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
//    Interrupt_enable(INT_TIMER1);
//    Interrupt_enable(INT_TIMER2);

    //
    // Starts CPU-Timer 0, CPU-Timer 1, and CPU-Timer 2.
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);
//    CPUTimer_startTimer(CPUTIMER1_BASE);
//    CPUTimer_startTimer(CPUTIMER2_BASE);
}


//
// initCPUTimers - This function initializes all three CPU timers
// to a known state.
//
void initCPUTimers(void)
{
    //
    // Initialize timer period to maximum
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
//    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);
//    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFF);

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
//    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
//    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);
//    CPUTimer_stopTimer(CPUTIMER1_BASE);
//    CPUTimer_stopTimer(CPUTIMER2_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
//    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
//    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);

    //
    // Reset interrupt counter
    //
    cpuTimer0IntCount = 0;
    cpuTimer1IntCount = 0;
    cpuTimer2IntCount = 0;
}

void InitDSP_Gpio(void)
{
    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    Device_initGPIO();

    //
    //Configure GPIO in Push Pull,Output Mode, digital pins
    //
    GPIO_setPadConfig(23U, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(23U, GPIO_DIR_MODE_OUT);
    GPIO_setAnalogMode(23U, GPIO_ANALOG_DISABLED);


    GPIO_setDirectionMode(34U, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(34U, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_34_GPIO34);

    GPIO_setControllerCore(23U, GPIO_CORE_CPU1_CLA1);
    GPIO_setControllerCore(34U, GPIO_CORE_CPU1_CLA1);


    GPIO_setPinConfig(GPIO_8_GPIO8);
    GPIO_setPadConfig(8, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);

    GPIO_setPinConfig(GPIO_9_GPIO9);
    GPIO_setPadConfig(9, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);


    GPIO_setPinConfig(GPIO_24_GPIO24);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_IN);

    GPIO_setPinConfig(GPIO_32_GPIO32);
    GPIO_setPadConfig(32, GPIO_PIN_TYPE_PULLUP);
    GPIO_setDirectionMode(32, GPIO_DIR_MODE_IN);



    // GPIO28 -> myGPIOInputInterrupt0 Pinmux
    GPIO_setPinConfig(GPIO_28_GPIO28);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(28, GPIO_DIR_MODE_IN);

    GPIO_setInterruptType(GPIO_INT_XINT1, GPIO_INT_TYPE_BOTH_EDGES);
    GPIO_setInterruptPin(28, GPIO_INT_XINT1);
    GPIO_enableInterrupt(GPIO_INT_XINT1);

}

void InitDSP_Epwm(void)
{
    //
    // Stop all PWM mode clock
    //
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // EPWM1 Pinmux
    //
    GPIO_setPinConfig(GPIO_0_EPWM1_A);
    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(0, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(GPIO_1_EPWM1_B);
    GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(1, GPIO_QUAL_SYNC);

    //
    // EPWM2 Pinmux
    //
    GPIO_setPinConfig(GPIO_2_EPWM2_A);
    GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(2, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(GPIO_3_EPWM2_B);
    GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(3, GPIO_QUAL_SYNC);


    //
    // Setup epwm1 TBCLK
    //
    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);
    EPWM_setTimeBasePeriod(EPWM1_BASE, 500);
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_disablePhaseShiftLoad(EPWM1_BASE);
    EPWM_setPhaseShift(EPWM1_BASE, 0);
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, 250);
    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_C, 350);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setDeadBandCounterClock(EPWM1_BASE,EPWM_DB_COUNTER_CLOCK_FULL_CYCLE);
    EPWM_setRisingEdgeDelayCount(EPWM1_BASE, EPWM1_DBT_200NS);
    EPWM_setFallingEdgeDelayCount(EPWM1_BASE, EPWM1_DBT_200NS);
    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_RED, true);
    EPWM_setDeadBandDelayMode(EPWM1_BASE, EPWM_DB_FED, true);
    EPWM_setRisingEdgeDeadBandDelayInput(EPWM1_BASE,EPWM_DB_INPUT_EPWMA);
    EPWM_setFallingEdgeDeadBandDelayInput(EPWM1_BASE,EPWM_DB_INPUT_EPWMA);
    EPWM_setDeadBandDelayPolarity(EPWM1_BASE,EPWM_DB_FED,EPWM_DB_POLARITY_ACTIVE_LOW);
    EPWM_setDeadBandDelayPolarity(EPWM1_BASE,EPWM_DB_RED,EPWM_DB_POLARITY_ACTIVE_HIGH);


    //
    // Setup epwm2 TBCLK
    //
    EPWM_setTimeBasePeriod(EPWM2_BASE, 1000);
    EPWM_setTimeBaseCounterMode(EPWM2_BASE, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_setTimeBaseCounter(EPWM2_BASE, 0U);
    EPWM_disablePhaseShiftLoad(EPWM2_BASE);
    EPWM_setPhaseShift(EPWM2_BASE, 0);
    EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                             EPWM_AQ_OUTPUT_A,
                                             EPWM_AQ_SW_OUTPUT_LOW);
    EPWM_setActionQualifierContSWForceAction(EPWM2_BASE,
                                             EPWM_AQ_OUTPUT_B,
                                             EPWM_AQ_SW_OUTPUT_LOW);

    //
    // Configure interrupt on event CTR=PERIOD
    //
    EPWM_enableInterrupt(EPWM1_BASE);
    EPWM_setInterruptSource(EPWM1_BASE, EPWM_INT_TBCTR_U_CMPC);
    EPWM_setInterruptEventCount(EPWM1_BASE, 2);

    //
    // Enable PWM Clocks
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Disable SOCA
    //
    EPWM_disableADCTrigger(EPWM1_BASE, EPWM_SOC_A);

    //
    // Configure the SOC to occur on the first up-count event
    //
    EPWM_setADCTriggerSource(EPWM1_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_U_CMPC);
    EPWM_setADCTriggerEventPrescale(EPWM1_BASE, EPWM_SOC_A, 1);

    //
    // Enable EPWM1 Interrupt
    //
    Interrupt_enable(INT_EPWM1);

    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(EPWM1_BASE, EPWM_SOC_A);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);

}

void InitDSP_Adc(void)
{
    //
    // asysctl initialization
    //
    // Disables the temperature sensor output to the ADC.
    //
    ASysCtl_disableTemperatureSensor();
    //
    // Set the analog voltage reference selection to internal.
    //
    ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
    //
    // Set the internal analog voltage reference selection to 1.65V.
    //
    ASysCtl_setAnalogReference1P65( ASYSCTL_VREFHIA | ASYSCTL_VREFHIB | ASYSCTL_VREFHIC );
    //
    // ADC Initialization: Write ADC configurations and power up the ADC
    //
    // Configures the ADC module's offset trim
    //
    ADC_setOffsetTrimAll(ADC_REFERENCE_INTERNAL,ADC_REFERENCE_3_3V);
    //
    // Configures the analog-to-digital converter module prescaler.
    //
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
    //
    // Sets the timing of the end-of-conversion pulse
    //
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    //
    // Powers up the analog-to-digital converter core.
    //
    ADC_enableConverter(ADCA_BASE);
    //
    // Delay for 5ms to allow ADC time to power up
    //
    DEVICE_DELAY_US(5000);
    //
    // SOC Configuration: Setup ADC EPWM channel and trigger settings
    //
    // Disables SOC burst mode.
    //
    ADC_disableBurstMode(ADCA_BASE);
    //
    // Sets the priority mode of the SOCs.
    //
    ADC_setSOCPriority(ADCA_BASE, ADC_PRI_ALL_ROUND_ROBIN);

    //
    // Start of Conversion 0 Configuration
    //
    //
    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 8 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0, 8U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 8 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2, 8U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER2, ADC_INT_SOC_TRIGGER_NONE);

    // Configures a start-of-conversion (SOC) in the ADC and its interrupt SOC trigger.
    //      SOC number      : 0
    //      Trigger         : ADC_TRIGGER_EPWM1_SOCA
    //      Channel         : ADC_CH_ADCIN0
    //      Sample Window   : 8 SYSCLK cycles
    //      Interrupt Trigger: ADC_INT_SOC_TRIGGER_NONE
    //
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN3, 8U);
    ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER3, ADC_INT_SOC_TRIGGER_NONE);

    //
    // ADC Interrupt 1 Configuration
    //      SOC/EOC number  : 0
    //      Interrupt Source: enabled
    //      Continuous Mode : disabled
    //
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);

//    //
//    //Setup conversion on ADC for PFC current and voltage.
//    //Vin_sense:A0
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0,
//                 ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN0,
//                 8);
//
//    //
//    //Iout:A1
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN1,
//                 8);
//
//    //
//    //Iin:A2
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2,
//                 ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2,
//                 8);
//
//    //
//    //Vbus:A3
//    //
////    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3,
////                 ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN3,
////                 8);
//
//    //
//    //Iout:A4
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN4,
//                 8);
//
//    //
//    //Temp2:A5
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN5,
//                 8);
//
//    //
//    //Temp2:A6
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER6,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN6,
//                 8);
//
//    //
//    //AC_L:A8
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER8,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN8,
//                 8);
//
//    //
//    //AC_N:A9
//    //
//    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER9,
//                 ADC_TRIGGER_CPU1_TINT1, ADC_CH_ADCIN9,
//                 8);
//
//    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
//    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
//    ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
//    Interrupt_enable(INT_ADCA1);
}

void InitDSP_Dac(void)
{
    //
    // Set DAC reference voltage.
    //
    DAC_setReferenceVoltage(DACB_BASE, DAC_REF_ADC_VREFHI);
    //
    // Set DAC load mode.
    //
    DAC_setLoadMode(DACB_BASE, DAC_LOAD_SYSCLK);
    //
    // Enable the DAC output
    //
    DAC_enableOutput(DACB_BASE);
    //
    // Set the DAC shadow output
    //
    DAC_setShadowValue(DACB_BASE, 0U);

    //
    // Delay for buffered DAC to power up.
    //
    DEVICE_DELAY_US(500);
}

void InitDSP_Scia(void)
{
    //
    // GPIO28 is the SCI Rx pin.
    //
    GPIO_setControllerCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    //
    // GPIO29 is the SCI Tx pin.
    //
    GPIO_setControllerCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    //
    // Map the ISR to the wake interrupt.
    //
    Interrupt_register(INT_SCIA_RX, sciaRxISR);

    //
    // Initialize SCIA and its FIFO.
    //
    SCI_performSoftwareReset(SCIA_BASE);

    //
    // Configure SCIA for echoback.
    //
    SCI_setConfig(SCIA_BASE, 25000000, 9600, (SCI_CONFIG_WLEN_8 |
                                             SCI_CONFIG_STOP_ONE |
                                             SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXRDY_BRKDT);
    SCI_enableModule(SCIA_BASE);
//    SCI_performSoftwareReset(SCIA_BASE);

    //
    // Clear the SCI interrupts before enabling them.
    //
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXRDY_BRKDT);

    //
    // Enable the interrupts in the PIE: Group 9 interrupts 1 & 2.
    //
    Interrupt_enable(INT_SCIA_RX);

    //
    // Enable the TXRDY and RXRDY interrupts.
    //
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_RXRDY_BRKDT);

    //
    // Clear the SCI ACK group.
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

void InitDSP_Can(void)
{
    //
    // Configure GPIO pin as CAN pin
    //
    GPIO_setPinConfig(GPIO_33_CANA_RX);
    GPIO_setPinConfig(GPIO_32_CANA_TX);

    //
    // Initialize the CAN controllers
    //
    CAN_initModule(CANA_BASE);

    //
    // Set up the CAN bus bit rate to 500kHz for each module
    // Refer to the Driver Library User Guide for information on how to set
    // tighter timing control. Additionally, consult the device data sheet
    // for more information about the CAN module clocking.
    //
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 20);

    //
    // Enable interrupts on the CAN peripheral.
    //
    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0 | CAN_INT_ERROR |
                        CAN_INT_STATUS);

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    // This registers the interrupt handler in PIE vector table.
    //
    Interrupt_register(INT_CANA0, &canISR);

    //
    // Enable the CAN interrupt signal
    Interrupt_enable(INT_CANA0);
    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    //
    // Initialize the transmit message object used for sending CAN messages.
    // Message Object Parameters:
    //      CAN Module: A
    //      Message Object ID Number: 1
    //      Message Identifier: 0x15555555
    //      Message Frame: Extended
    //      Message Type: Transmit
    //      Message ID Mask: 0x0
    //      Message Object Flags: None
    //      Message Data Length: 4 Bytes (Note that DLC field is a "don't care"
    //      for a Receive mailbox
    //
    CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID1, 0x10000,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0,
                           CAN_MSG_OBJ_NO_FLAGS, MSG_DATA_LENGTH);

    CAN_setupMessageObject(CANA_BASE, RX_MSG_OBJ_ID, 0x60000,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_RX, 0,
                           CAN_MSG_OBJ_RX_INT_ENABLE, MSG_DATA_LENGTH);

    //
    // Start CAN module A operations
    //
    CAN_startModule(CANA_BASE);
}

void InitDSP_Interrupt(void)
{
    //
    // ISRs for each CPU Timer interrupt
    //
    Interrupt_register(INT_TIMER0, &cpuTimer0ISR);
//    Interrupt_register(INT_TIMER1, &cpuTimer1ISR);
//    Interrupt_register(INT_TIMER2, &cpuTimer2ISR);

    //
    // ISRs for epwm1 interrupt
    //
    Interrupt_register(INT_EPWM1, &epwm1ISR);

    //
    // ISRs for gpio interrupt
    //
    Interrupt_register(INT_XINT1, &gpioISR);

    //
    // ISRs for adc Timer interrupt
    //
//    Interrupt_register(INT_ADCA1, &adcISR);
}

void InitDSP_Cla()
{

#ifdef _FLASH
#ifndef CMDTOOL // Linker command tool is not used

    extern uint32_t Cla1ProgRunStart, Cla1ProgLoadStart, Cla1ProgLoadSize;
    extern uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;

    //
    // Copy the program and constants from FLASH to RAM before configuring
    // the CLA
    //
    memcpy((uint32_t *)&Cla1ProgRunStart, (uint32_t *)&Cla1ProgLoadStart,
           (uint32_t)&Cla1ProgLoadSize);
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
        (uint32_t)&Cla1ConstLoadSize );


#endif //CMDTOOL
#endif //_FLASH
    //
    // Configure all CLA task vectors
    // On Type-1 and Type-2 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
#pragma diag_suppress=770
    //
    // CLA Task 1
    //
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_1, (uint16_t)&Cla1Task1);
    CLA_setTriggerSource(CLA_TASK_1, CLA_TRIGGER_EPWM1INT);
    //
    // CLA Task 8
    //
    CLA_mapBackgroundTaskVector(CLA1_BASE, (uint16_t)&Cla1BackgroundTask);
    CLA_enableHardwareTrigger(CLA1_BASE);
    CLA_setTriggerSource(CLA_TASK_8, CLA_TRIGGER_TINT0);
    //
    // Enable background task
    //
    CLA_enableBackgroundTask(CLA1_BASE);
#pragma diag_warning=770
    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    CLA_enableIACK(CLA1_BASE);
    CLA_enableTasks(CLA1_BASE, CLA_TASKFLAG_ALL);
}

//*****************************************************************************
//
// MEMCFG Configurations
//
//*****************************************************************************
void InitDSP_MemCfg(){
    //
    // Initialize RAMs
    //
    MemCfg_initSections(MEMCFG_SECT_MSGCPUTOCLA1);
    MemCfg_initSections(MEMCFG_SECT_MSGCLA1TOCPU);
    while(!MemCfg_getInitStatus(MEMCFG_SECT_MSGCPUTOCLA1));
    while(!MemCfg_getInitStatus(MEMCFG_SECT_MSGCLA1TOCPU));
    //
    // Configure LSRAMs
    //
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS0, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS0, MEMCFG_CLA_MEM_PROGRAM);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS1, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS1, MEMCFG_CLA_MEM_DATA);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS2, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS2, MEMCFG_CLA_MEM_DATA);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS3, MEMCFG_LSRAMCONTROLLER_CPU_CLA1);
    MemCfg_setCLAMemType(MEMCFG_SECT_LS3, MEMCFG_CLA_MEM_DATA);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS4, MEMCFG_LSRAMCONTROLLER_CPU_ONLY);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS5, MEMCFG_LSRAMCONTROLLER_CPU_ONLY);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS6, MEMCFG_LSRAMCONTROLLER_CPU_ONLY);
    MemCfg_setLSRAMControllerSel(MEMCFG_SECT_LS7, MEMCFG_LSRAMCONTROLLER_CPU_ONLY);
    //
    // Configure GSRAMs
    //
    //
    // Configure Access Protection for RAMs
    //
    MemCfg_setProtection(MEMCFG_SECT_LS0, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS1, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS2, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS3, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS4, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS5, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS6, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_LS7, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE);
    MemCfg_setProtection(MEMCFG_SECT_GS0, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE | MEMCFG_PROT_ALLOWDMAWRITE);
    MemCfg_setProtection(MEMCFG_SECT_GS1, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE | MEMCFG_PROT_ALLOWDMAWRITE);
    MemCfg_setProtection(MEMCFG_SECT_GS2, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE | MEMCFG_PROT_ALLOWDMAWRITE);
    MemCfg_setProtection(MEMCFG_SECT_GS3, MEMCFG_PROT_ALLOWCPUFETCH | MEMCFG_PROT_ALLOWCPUWRITE | MEMCFG_PROT_ALLOWDMAWRITE);
    //
    // Lock/Commit Registers
    //
    //
    // Enable Access Violation Interrupt
    //
    //
    // Correctable error Interrupt
    //
    MemCfg_setCorrErrorThreshold(0);
    MemCfg_disableCorrErrorInterrupt(MEMCFG_CERR_CPUREAD);
}

//
// configCPUTimer - This function initializes the selected timer to the
// period specified by the "freq" and "period" parameters. The "freq" is
// entered as Hz and the period in uSeconds. The timer is held in the stopped
// state after configuration.
//
void
configCPUTimer(uint32_t cpuTimer, float freq, float period)
{
    uint32_t temp;

    //
    // Initialize timer period:
    //
    temp = (uint32_t)((freq / 1000000) * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(cpuTimer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer,
                              CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(cpuTimer);
}


//
// cpuTimer0ISR - Counter for CpuTimer0
//
__interrupt void cpuTimer0ISR(void)
{
//    cpuTimer0IntCount++;

//    CAN_sendMessage(CANA_BASE, 1, MSG_DATA_LENGTH, rxMsgData);
//
//    GPIO_togglePin(23);
//
//    msg = "\r\n\n\nHello World!\0";
//    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 17);


    TIMEBASE_1MS_HANDLER();

    cpuTimer0IntCount++;
    if(cpuTimer0IntCount >= 1000)
    {
        cpuTimer0IntCount = 0;
//        GPIO_togglePin(23);
    }


    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//
// cpuTimer1ISR - Counter for CpuTimer1
//
//__interrupt void cpuTimer1ISR(void)
//{
//
////     The CPU acknowledges the interrupt.
//
//    cpuTimer1IntCount++;
//    if(cpuTimer1IntCount >= 10000)
//    {
//        cpuTimer1IntCount = 0;
//        GPIO_togglePin(9);
//        GPIO_togglePin(23);
//    }
//
////    Adc_Cal_10KHz();
////    WarnCtrl_Handler();
//
//
//}

//
// cpuTimer2ISR - Counter for CpuTimer2
//
//__interrupt void cpuTimer2ISR(void)
//{
//
////     The CPU acknowledges the interrupt.
//
//    cpuTimer2IntCount++;
//    if(cpuTimer2IntCount >= 500)
//    {
//        cpuTimer2IntCount = 0;
////        GPIO_togglePin(8);
//    }
//
////    TIMEBASE_1MS_HANDLER();
//
//}

//
// CAN ISR - The interrupt service routine called when a CAN interrupt is
//           triggered.  It checks for the cause of the interrupt, and
//           maintains a count of all messages that have been transmitted.
//
__interrupt void canISR(void)
{
    uint32_t status;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    status = CAN_getInterruptCause(CANA_BASE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(status == CAN_INT_INT0ID_STATUS)
    {

        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        //
        status = CAN_getStatus(CANA_BASE);

        //
        // Check to see if an error occurred.
        //
        if(((status  & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 7) &&
           ((status  & ~(CAN_STATUS_TXOK | CAN_STATUS_RXOK)) != 0))
        {
            //
            // Set a flag to indicate some errors may have occurred.
            //
//            errorFlag = 1;
        }
    }

    //
    // Check if the cause is the receive message object 2
    //
    else if(status == RX_MSG_OBJ_ID)
    {

        //
        // Get the received message
        //
        CAN_readMessage(CANA_BASE, RX_MSG_OBJ_ID, rxMsgData);



        //
        // Getting to this point means that the RX interrupt occurred on
        // message object 2, and the message RX is complete.  Clear the
        // message object interrupt.
        //
        CAN_clearInterruptStatus(CANA_BASE, RX_MSG_OBJ_ID);

    }

    //
    // If something unexpected caused the interrupt, this would handle it.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }

    //
    // Clear the global interrupt flag for the CAN interrupt line
    //
    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    //
    // Acknowledge this interrupt located in group 9
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

//
// sciaRxISR - Read the character from the RXBUF and echo it back.
//
__interrupt void sciaRxISR(void)
{
    uint16_t receivedChar;

    //
    // Enable the TXRDY interrupt again.
    //
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_TXRDY);

    //
    // Read a character from the RXBUF.
    //
    receivedChar = SCI_readCharBlockingNonFIFO(SCIA_BASE);

    //
    // Echo back the character.
    //
    msg = "  You sent: \0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 13);
    SCI_writeCharBlockingNonFIFO(SCIA_BASE, receivedChar);

    //
    // Acknowledge the PIE interrupt.
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

#pragma CODE_SECTION( epwm1ISR, ".TI.ramfunc" );
__interrupt void epwm1ISR(void)
{
    //There is a issues that PC can't to enter main loop
    //Maybe this ISR cost the run time more than interrupt time
    //At present, this interrupt call time is 20us

//    GPIO_togglePin(23);
//    GPIO_writePin(23,1);


    Adc_Cal_100KHz();//2.45us
    VoltLoopCtrl();
    Iref_Cal();//1.6us
    CurrLoopCtrl();//1.72us
    PWMDriver_Ctrl();//5us

//    epwmIntCount++;
//    if(epwmIntCount >= 100000)
//    {
//        epwmIntCount = 0;
//        CAN_sendMessage(CANA_BASE, 1, MSG_DATA_LENGTH, rxMsgData);
//        GPIO_togglePin(23);
//        TimerFlag1s = 1;
//
//    }
//    GPIO_writePin(23,0);
    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(EPWM1_BASE);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);

}

__interrupt void gpioISR(void)
{
    //
    //High level to low is represent positive cross zero
    //Low level to high is represent negative cross zero
    //
    if(GPIO_readPin(GPIO_DETECT_AC_DIRECTION))
    {
        g_u8GridStatus = POSITION_CROSS_ZERO;
    }
    else
    {
        g_u8GridStatus = NEGTIVE_CROSS_ZERO;
    }
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

//__interrupt void adcISR(void)
//{
//    static uint32_t u32Delay;
//
//    Adc_Cal_10KHz();
//
//    u32Delay++;
//    if(u32Delay >= 10000)
//    {
//        u32Delay = 0;
////        GPIO_togglePin(23);
////        GPIO_togglePin(8);
//    }
//
//    //
//    // Clear the interrupt flag
//    //
//    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
//
//    //
//    // Check if overflow has occurred
//    //
//    if(1 == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1))
//    {
//        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
//        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
//    }
//
//    //
//    // Acknowledge the interrupt
//    //
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
//}

//
// End of File
//
