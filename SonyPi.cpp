/*
 * Under GNU License
 * Date 6-16-2009
 */

#include "SonyPi.h"

#define super IOPCIBridge

static struct sonypi_ioport_list sonypi_type1_ioport_list[] = {
	{ 0x10c0, 0x10c4 },	/* looks like the default on C1Vx */
	{ 0x1080, 0x1084 },
	{ 0x1090, 0x1094 },
	{ 0x10a0, 0x10a4 },
	{ 0x10b0, 0x10b4 },
	{ 0x0, 0x0 }
};

static struct sonypi_ioport_list sonypi_type2_ioport_list[] = {
	{ 0x1080, 0x1084 },
	{ 0x10a0, 0x10a4 },
	{ 0x10c0, 0x10c4 },
	{ 0x10e0, 0x10e4 },
	{ 0x0, 0x0 }
};

/* same as in type 2 models */
static struct sonypi_ioport_list *sonypi_type3_ioport_list =
	sonypi_type2_ioport_list;

static struct sonypi_irq_list sonypi_type1_irq_list[] = {
	{ 11, 0x2 },	/* IRQ 11, GO22=0,GO23=1 in AML */
	{ 10, 0x1 },	/* IRQ 10, GO22=1,GO23=0 in AML */
	{  5, 0x0 },	/* IRQ  5, GO22=0,GO23=0 in AML */
	{  0, 0x3 }	/* no IRQ, GO22=1,GO23=1 in AML */
};

static struct sonypi_irq_list sonypi_type2_irq_list[] = {
	{ 11, 0x80 },	/* IRQ 11, 0x80 in SIRQ in AML */
	{ 10, 0x40 },	/* IRQ 10, 0x40 in SIRQ in AML */
	{  9, 0x20 },	/* IRQ  9, 0x20 in SIRQ in AML */
	{  6, 0x10 },	/* IRQ  6, 0x10 in SIRQ in AML */
	{  0, 0x00 }	/* no IRQ, 0x00 in SIRQ in AML */
};

/* same as in type2 models */
static struct sonypi_irq_list *sonypi_type3_irq_list = sonypi_type2_irq_list;


/* The set of possible button release events */
static struct sonypi_event sonypi_releaseev[] = {
	{ 0x00, SONYPI_EVENT_ANYBUTTON_RELEASED },
	{ 0, 0 }
};

/* The set of possible jogger events  */
static struct sonypi_event sonypi_joggerev[] = {
	{ 0x1f, SONYPI_EVENT_JOGDIAL_UP },
	{ 0x01, SONYPI_EVENT_JOGDIAL_DOWN },
	{ 0x5f, SONYPI_EVENT_JOGDIAL_UP_PRESSED },
	{ 0x41, SONYPI_EVENT_JOGDIAL_DOWN_PRESSED },
	{ 0x1e, SONYPI_EVENT_JOGDIAL_FAST_UP },
	{ 0x02, SONYPI_EVENT_JOGDIAL_FAST_DOWN },
	{ 0x5e, SONYPI_EVENT_JOGDIAL_FAST_UP_PRESSED },
	{ 0x42, SONYPI_EVENT_JOGDIAL_FAST_DOWN_PRESSED },
	{ 0x1d, SONYPI_EVENT_JOGDIAL_VFAST_UP },
	{ 0x03, SONYPI_EVENT_JOGDIAL_VFAST_DOWN },
	{ 0x5d, SONYPI_EVENT_JOGDIAL_VFAST_UP_PRESSED },
	{ 0x43, SONYPI_EVENT_JOGDIAL_VFAST_DOWN_PRESSED },
	{ 0x40, SONYPI_EVENT_JOGDIAL_PRESSED },
	{ 0, 0 }
};

/* The set of possible capture button events */
static struct sonypi_event sonypi_captureev[] = {
	{ 0x05, SONYPI_EVENT_CAPTURE_PARTIALPRESSED },
	{ 0x07, SONYPI_EVENT_CAPTURE_PRESSED },
	{ 0x01, SONYPI_EVENT_CAPTURE_PARTIALRELEASED },
	{ 0, 0 }
};

/* The set of possible fnkeys events */
static struct sonypi_event sonypi_fnkeyev[] = {
	{ 0x10, SONYPI_EVENT_FNKEY_ESC },
	{ 0x11, SONYPI_EVENT_FNKEY_F1 },
	{ 0x12, SONYPI_EVENT_FNKEY_F2 },
	{ 0x13, SONYPI_EVENT_FNKEY_F3 },
	{ 0x14, SONYPI_EVENT_FNKEY_F4 },
	{ 0x15, SONYPI_EVENT_FNKEY_F5 },
	{ 0x16, SONYPI_EVENT_FNKEY_F6 },
	{ 0x17, SONYPI_EVENT_FNKEY_F7 },
	{ 0x18, SONYPI_EVENT_FNKEY_F8 },
	{ 0x19, SONYPI_EVENT_FNKEY_F9 },
	{ 0x1a, SONYPI_EVENT_FNKEY_F10 },
	{ 0x1b, SONYPI_EVENT_FNKEY_F11 },
	{ 0x1c, SONYPI_EVENT_FNKEY_F12 },
	{ 0x1f, SONYPI_EVENT_FNKEY_RELEASED },
	{ 0x21, SONYPI_EVENT_FNKEY_1 },
	{ 0x22, SONYPI_EVENT_FNKEY_2 },
	{ 0x31, SONYPI_EVENT_FNKEY_D },
	{ 0x32, SONYPI_EVENT_FNKEY_E },
	{ 0x33, SONYPI_EVENT_FNKEY_F },
	{ 0x34, SONYPI_EVENT_FNKEY_S },
	{ 0x35, SONYPI_EVENT_FNKEY_B },
	{ 0x36, SONYPI_EVENT_FNKEY_ONLY },
	{ 0, 0 }
};

/* The set of possible program key events */
static struct sonypi_event sonypi_pkeyev[] = {
	{ 0x01, SONYPI_EVENT_PKEY_P1 },
	{ 0x02, SONYPI_EVENT_PKEY_P2 },
	{ 0x04, SONYPI_EVENT_PKEY_P3 },
	{ 0x5c, SONYPI_EVENT_PKEY_P1 },
	{ 0, 0 }
};

/* The set of possible bluetooth events */
static struct sonypi_event sonypi_blueev[] = {
	{ 0x55, SONYPI_EVENT_BLUETOOTH_PRESSED },
	{ 0x59, SONYPI_EVENT_BLUETOOTH_ON },
	{ 0x5a, SONYPI_EVENT_BLUETOOTH_OFF },
	{ 0, 0 }
};

/* The set of possible wireless events */
static struct sonypi_event sonypi_wlessev[] = {
	{ 0x59, SONYPI_EVENT_WIRELESS_ON },
	{ 0x5a, SONYPI_EVENT_WIRELESS_OFF },
	{ 0, 0 }
};

/* The set of possible back button events */
static struct sonypi_event sonypi_backev[] = {
	{ 0x20, SONYPI_EVENT_BACK_PRESSED },
	{ 0, 0 }
};

/* The set of possible help button events */
static struct sonypi_event sonypi_helpev[] = {
	{ 0x3b, SONYPI_EVENT_HELP_PRESSED },
	{ 0, 0 }
};


/* The set of possible lid events */
static struct sonypi_event sonypi_lidev[] = {
	{ 0x51, SONYPI_EVENT_LID_CLOSED },
	{ 0x50, SONYPI_EVENT_LID_OPENED },
	{ 0, 0 }
};

/* The set of possible zoom events */
static struct sonypi_event sonypi_zoomev[] = {
	{ 0x39, SONYPI_EVENT_ZOOM_PRESSED },
	{ 0, 0 }
};

/* The set of possible thumbphrase events */
static struct sonypi_event sonypi_thumbphraseev[] = {
	{ 0x3a, SONYPI_EVENT_THUMBPHRASE_PRESSED },
	{ 0, 0 }
};

/* The set of possible motioneye camera events */
static struct sonypi_event sonypi_meyeev[] = {
	{ 0x00, SONYPI_EVENT_MEYE_FACE },
	{ 0x01, SONYPI_EVENT_MEYE_OPPOSITE },
	{ 0, 0 }
};

/* The set of possible memorystick events */
static struct sonypi_event sonypi_memorystickev[] = {
	{ 0x53, SONYPI_EVENT_MEMORYSTICK_INSERT },
	{ 0x54, SONYPI_EVENT_MEMORYSTICK_EJECT },
	{ 0, 0 }
};

/* The set of possible battery events */
static struct sonypi_event sonypi_batteryev[] = {
	{ 0x20, SONYPI_EVENT_BATTERY_INSERT },
	{ 0x30, SONYPI_EVENT_BATTERY_REMOVE },
	{ 0, 0 }
};


struct sonypi_eventtypes sonypi_eventtypes[] = {
	{ SONYPI_DEVICE_MODEL_TYPE1, 0, 0xffffffff, sonypi_releaseev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x70, SONYPI_MEYE_MASK, sonypi_meyeev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x30, SONYPI_LID_MASK, sonypi_lidev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x60, SONYPI_CAPTURE_MASK, sonypi_captureev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x10, SONYPI_JOGGER_MASK, sonypi_joggerev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x20, SONYPI_FNKEY_MASK, sonypi_fnkeyev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x30, SONYPI_BLUETOOTH_MASK, sonypi_blueev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x40, SONYPI_PKEY_MASK, sonypi_pkeyev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x30, SONYPI_MEMORYSTICK_MASK, sonypi_memorystickev },
	{ SONYPI_DEVICE_MODEL_TYPE1, 0x40, SONYPI_BATTERY_MASK, sonypi_batteryev },

	{ SONYPI_DEVICE_MODEL_TYPE2, 0, 0xffffffff, sonypi_releaseev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x38, SONYPI_LID_MASK, sonypi_lidev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x11, SONYPI_JOGGER_MASK, sonypi_joggerev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x61, SONYPI_CAPTURE_MASK, sonypi_captureev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x21, SONYPI_FNKEY_MASK, sonypi_fnkeyev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x31, SONYPI_BLUETOOTH_MASK, sonypi_blueev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x08, SONYPI_PKEY_MASK, sonypi_pkeyev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x11, SONYPI_BACK_MASK, sonypi_backev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x21, SONYPI_HELP_MASK, sonypi_helpev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x21, SONYPI_ZOOM_MASK, sonypi_zoomev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x20, SONYPI_THUMBPHRASE_MASK, sonypi_thumbphraseev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x31, SONYPI_MEMORYSTICK_MASK, sonypi_memorystickev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x41, SONYPI_BATTERY_MASK, sonypi_batteryev },
	{ SONYPI_DEVICE_MODEL_TYPE2, 0x31, SONYPI_PKEY_MASK, sonypi_pkeyev },

	{ SONYPI_DEVICE_MODEL_TYPE3, 0, 0xffffffff, sonypi_releaseev },
	{ SONYPI_DEVICE_MODEL_TYPE3, 0x21, SONYPI_FNKEY_MASK, sonypi_fnkeyev },
	{ SONYPI_DEVICE_MODEL_TYPE3, 0x31, SONYPI_WIRELESS_MASK, sonypi_wlessev },
	{ SONYPI_DEVICE_MODEL_TYPE3, 0x31, SONYPI_MEMORYSTICK_MASK, sonypi_memorystickev },
	{ SONYPI_DEVICE_MODEL_TYPE3, 0x41, SONYPI_BATTERY_MASK, sonypi_batteryev },
	{ SONYPI_DEVICE_MODEL_TYPE3, 0x31, SONYPI_PKEY_MASK, sonypi_pkeyev },
	{ 0 }
};

struct sonypi_input_ev sonypi_inputkeys[] = {
	{ SONYPI_EVENT_CAPTURE_PRESSED,	 	KEY_CAMERA },
	{ SONYPI_EVENT_FNKEY_ONLY, 		KEY_FN },
	{ SONYPI_EVENT_FNKEY_ESC, 		KEY_FN_ESC },
	{ SONYPI_EVENT_FNKEY_F1, 		KEY_FN_F1 },
	{ SONYPI_EVENT_FNKEY_F2, 		KEY_FN_F2 },
	{ SONYPI_EVENT_FNKEY_F3, 		KEY_FN_F3 },
	{ SONYPI_EVENT_FNKEY_F4, 		KEY_FN_F4 },
	{ SONYPI_EVENT_FNKEY_F5, 		KEY_FN_F5 },
	{ SONYPI_EVENT_FNKEY_F6, 		KEY_FN_F6 },
	{ SONYPI_EVENT_FNKEY_F7, 		KEY_FN_F7 },
	{ SONYPI_EVENT_FNKEY_F8, 		KEY_FN_F8 },
	{ SONYPI_EVENT_FNKEY_F9,		KEY_FN_F9 },
	{ SONYPI_EVENT_FNKEY_F10,		KEY_FN_F10 },
	{ SONYPI_EVENT_FNKEY_F11, 		KEY_FN_F11 },
	{ SONYPI_EVENT_FNKEY_F12,		KEY_FN_F12 },
	{ SONYPI_EVENT_FNKEY_1, 		KEY_FN_1 },
	{ SONYPI_EVENT_FNKEY_2, 		KEY_FN_2 },
	{ SONYPI_EVENT_FNKEY_D,			KEY_FN_D },
	{ SONYPI_EVENT_FNKEY_E,			KEY_FN_E },
	{ SONYPI_EVENT_FNKEY_F,			KEY_FN_F },
	{ SONYPI_EVENT_FNKEY_S,			KEY_FN_S },
	{ SONYPI_EVENT_FNKEY_B,			KEY_FN_B },
	{ SONYPI_EVENT_BLUETOOTH_PRESSED, 	KEY_BLUE },
	{ SONYPI_EVENT_BLUETOOTH_ON, 		KEY_BLUE },
	{ SONYPI_EVENT_PKEY_P1, 		KEY_PROG1 },
	{ SONYPI_EVENT_PKEY_P2, 		KEY_PROG2 },
	{ SONYPI_EVENT_PKEY_P3, 		KEY_PROG3 },
	{ SONYPI_EVENT_BACK_PRESSED, 		KEY_BACK },
	{ SONYPI_EVENT_HELP_PRESSED, 		KEY_HELP },
	{ SONYPI_EVENT_ZOOM_PRESSED, 		KEY_ZOOM },
	{ SONYPI_EVENT_THUMBPHRASE_PRESSED, 	BTN_THUMB },
	{ 0, 0 },
};


OSDefineMetaClassAndStructors(SonyPiClass, IOPCIBridge)


bool
SonyPiClass::start( IOService * provider )
{
    return super::start(provider);
}

bool 
SonyPiClass::configure( IOService * provider )
{
	return super::configure( provider );
}

void 
SonyPiClass::free()
{
    super::free();
}

IODeviceMemory *
SonyPiClass::ioDeviceMemory( void )
{
    return( NULL);
}

UInt32 
SonyPiClass::configRead32( IOPCIAddressSpace space, UInt8 offset )
{
	return 0;
}

void 
SonyPiClass::configWrite32( IOPCIAddressSpace space, UInt8 offset, UInt32 data )
{
}

UInt16 
SonyPiClass::configRead16( IOPCIAddressSpace space, UInt8 offset )
{
	return 0;
}
void 
SonyPiClass::configWrite16( IOPCIAddressSpace space,
					UInt8 offset, UInt16 data )
{
	return 0;
}

UInt8 SonyPiClass::configRead8( IOPCIAddressSpace space, UInt8 offset )
{
	return 0;
}

void 
SonyPiClass::configWrite8( IOPCIAddressSpace space,
					UInt8 offset, UInt8 data )
{
	return 0;
}

IOReturn 
SonyPiClass::setDevicePowerState( IOPCIDevice * device,
                                          unsigned long whatToDo )
{
	return super::setDevicePowerState(device, whatToDo);;
}
