#include "pch.h"
#include "defaults.h"
//#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::D3; // ok
	engineConfiguration->injectionPins[1] = Gpio::A9; // ok
	engineConfiguration->injectionPins[2] = Gpio::D11; // ok
	engineConfiguration->injectionPins[3] = Gpio::D10; // ok

}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::C13; // ok
	engineConfiguration->ignitionPins[1] = Gpio::E5; // ok

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
	engineConfiguration->vbattDividerCoeff = (6.34 + 1) / 1; //TBD

	// Battery sense
	engineConfiguration->vbattAdcChannel = EFI_ADC_11; //DONE // PC1

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
	//setHellenMegaEnPin(); //TODO
	setupVbatt(); //done

	setSdCardSpi1(); //ok
	//hellenMegaAccelerometerPreInitCS2Pin(); //TODO

	//setHellenCan(); //below

	//setDefaultHellenAtPullUps(); // TODO
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
	engineConfiguration->etbIo[0].controlPin = Gpio::B8; //TBD
	// DIR pin
	engineConfiguration->etbIo[0].directionPin1 = Gpio::B9; //TBD
	// Disable pin
	engineConfiguration->etbIo[0].disablePin = Gpio::B7; //TBD


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

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4; // ok // PA4
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_1; // ok // PA1
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10; // ok // PC0

	setPPSInputs(EFI_ADC_3, EFI_ADC_14); // ok // PA3, PC4
	engineConfiguration->enableAemXSeries = false; // we will see

	engineConfiguration->clt.adcChannel = EFI_ADC_12; // ok // PC2

	engineConfiguration->iat.adcChannel = EFI_ADC_13; // ok // PC3

	engineConfiguration->triggerInputPins[0] = Gpio::B1; // VR2 max9924 is the safer default // TBD
	engineConfiguration->camInputs[0] = Gpio::A6; // HALL1 // TBD

  engineConfiguration->vehicleSpeedSensorInputPin = Gpio::E11; // ok
//baro sensor
	engineConfiguration->lps25BaroSensorScl = Gpio::B10;
	engineConfiguration->lps25BaroSensorSda = Gpio::B11;


  //setHellenMMbaro(); // TODO

	engineConfiguration->displayLogicLevelsInEngineSniffer = true; // ok

	engineConfiguration->globalTriggerAngleOffset = 0; // ok

	engineConfiguration->enableSoftwareKnock = true; // ok

	engineConfiguration->canTxPin = Gpio::D1; // ok
	engineConfiguration->canRxPin = Gpio::D0; // ok

    engineConfiguration->mainRelayPin = Gpio::C9; // ok
 	engineConfiguration->fanPin = Gpio::A8; // ok
	engineConfiguration->fuelPumpPin = Gpio::D13; // ok
	engineConfiguration->malfunctionIndicatorPin = Gpio::D14; // OK
	engineConfiguration->tachOutputPin = Gpio::D15;

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
	Gpio::A8, // B1 injector output 6
	Gpio::D2, // B2 injector output 5
	Gpio::D10, // B3 injector output 4
	Gpio::D11, // B4 injector output 3
	Gpio::A9, // B5 injector output 2
	Gpio::D3, // B6 injector output 1
	Gpio::D15, // B7 Low Side output 1
	Gpio::E6, // B8 Fan Relay Weak Low Side output 2
	Gpio::B9, // B9 Main Relay Weak Low Side output 1
	Gpio::C6, // B16 Low Side output 4 / Fuel Pump
	Gpio::D13, // B17 Low Side output 3
	Gpio::D12, // B18 Low Side output 2
	// high sides
	Gpio::B8, // B10 Coil 6
	Gpio::E3, // B11 Coil 4
	Gpio::E4, // B12 Coil 3
	Gpio::E2, // B13 Coil 5
	Gpio::E5, // B14 Coil 2
	Gpio::C13, // B15 Coil 1
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
	return Gpio::E7;
}

Gpio getRunningLedPin() {
	return Gpio::D7;
}

Gpio getWarningLedPin() {
	return Gpio::E8;
}