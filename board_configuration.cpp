#include "pch.h"
#include "defaults.h"
//#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::D12; 
	engineConfiguration->injectionPins[1] = Gpio::D13; 
	engineConfiguration->injectionPins[2] = Gpio::D11; 
	engineConfiguration->injectionPins[3] = Gpio::D10; 

}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::A8; 
	engineConfiguration->ignitionPins[1] = Gpio::A9; 

}


// static void init5vpDiag() {
// #ifdef DIAG_5VP_PIN
// static bool is5vpInit = false;
//   if (!is5vpInit) {
//     efiSetPadMode("5VP_STATE", DIAG_5VP_PIN, PAL_MODE_INPUT);
//     is5vpInit = true;
//   }
// #endif // DIAG_5VP_PIN
// }

static void setupVbatt() {
	// 2.7k high side/2.7k low side = 2 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f; // TBD  - check on dev board
	
	// 6.34k high side/ 1k low side
	engineConfiguration->vbattDividerCoeff = ((33.0 + 4.7) / 4.7)*2; //TBD

	// Battery sense
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;

	engineConfiguration->adcVcc = 3.3f;
}

void enableSpi1() {
	engineConfiguration->spi1mosiPin = Gpio::B5;
	engineConfiguration->spi1misoPin = Gpio::B4;
	engineConfiguration->spi1sckPin = Gpio::B3;
	engineConfiguration->is_enabled_spi_1 = true;
}

static void setSdCardSpi1NoCS() {
    engineConfiguration->isSdCardEnabled = true;
	engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;
	enableSpi1();
}

static void setSdCardSpi1() {
    setSdCardSpi1NoCS();
    engineConfiguration->sdCardCsPin = Gpio::B6;
}

void setBoardConfigOverrides() {
	
	setupVbatt(); //done

	setSdCardSpi1(); 
	
	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

}

static void setEtbConfig() {
	// TLE9201 driver
	// This chip has three control pins:
	// DIR - sets direction of the motor
	// PWM - pwm control (enable high, coast low)
	// DIS - disables motor (enable low)

	// Throttle #1
	// PWM pin
	engineConfiguration->etbIo[0].controlPin = Gpio::D15; 
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::C6; 
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::D14; 


	// we only have pwm/dir, no dira/dirb
	engineConfiguration->etb_use_two_wires = false;
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();
	setEtbConfig();

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_12;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_3;
	engineConfiguration->ignitionKeyDigitalPin = Gpio::E15;
	

	setPPSInputs(EFI_ADC_1, EFI_ADC_0);
	engineConfiguration->enableAemXSeries = false; // we will see

	engineConfiguration->clt.adcChannel = EFI_ADC_11;

	engineConfiguration->iat.adcChannel = EFI_ADC_10;

	engineConfiguration->triggerInputPins[0] = Gpio::E4; 
	engineConfiguration->camInputs[0] = Gpio::E5; 

  engineConfiguration->vehicleSpeedSensorInputPin = Gpio::E6; 
//baro sensor
	engineConfiguration->lps25BaroSensorScl = Gpio::B10;
	engineConfiguration->lps25BaroSensorSda = Gpio::B11;

// SPI3 for onboard EGT

	engineConfiguration->is_enabled_spi_3 = true;
	engineConfiguration->spi3misoPin = Gpio::C11;
	engineConfiguration->spi3sckPin = Gpio::C10;

  //setHellenMMbaro(); // TODO

	engineConfiguration->displayLogicLevelsInEngineSniffer = true; // ok

	engineConfiguration->globalTriggerAngleOffset = 0; // ok

	engineConfiguration->enableSoftwareKnock = true; // ok

	engineConfiguration->canTxPin = Gpio::D1; 
	engineConfiguration->canRxPin = Gpio::D0; 

    engineConfiguration->mainRelayPin = Gpio::B15; // ok
 	engineConfiguration->fanPin = Gpio::B13; // ok
	engineConfiguration->fuelPumpPin = Gpio::D8; // ok
	engineConfiguration->malfunctionIndicatorPin = Gpio::D9; // OK
	engineConfiguration->tachOutputPin = Gpio::B14;

	// "required" hardware is done - set some reasonable defaults
	
	engineConfiguration->enableVerboseCanTx = true; // ok

	engineConfiguration->etbFunctions[0] = DC_Throttle1;

	// Some sensible defaults for other options
	setCrankOperationMode(); //TODO

	setAlgorithm(LM_SPEED_DENSITY); //TODO

	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure; //TODO

	setCommonNTCSensor(&engineConfiguration->clt, 4700); //TODO
	setCommonNTCSensor(&engineConfiguration->iat, 4700); //TODO

    setTPS1Calibration(100, 650);
	//hellenWbo(); TBD
	engineConfiguration->enableAemXSeries = true; // Wideband CAN
}

static Gpio OUTPUTS[] = {
	Gpio::D8, 
	Gpio::D9, 
	Gpio::D10, 
	Gpio::D11, 
	Gpio::D12, 
	Gpio::D13, 
	Gpio::B15, 
	Gpio::B14, 
	Gpio::B13, 
	Gpio::A8, 
	Gpio::A9, 
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
  return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}
Gpio getCommsLedPin() {
	return Gpio::A7; // LED3
}

Gpio getRunningLedPin() {
	return Gpio::A6; // LED2
}

Gpio getWarningLedPin() {
	return Gpio::A5; // LED4
}