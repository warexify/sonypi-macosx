/*
 * Under GNU License
 */


#ifndef _IOKIT_SONY_PI_H
#define _IOKIT_SONY_PI_H

#include <IOKit/system.h>
#include <ppc/proc_reg.h>

#include <libkern/c++/OSContainers.h>
#include <libkern/OSByteOrder.h>

#include <IOKit/IODeviceMemory.h>
#include <IOKit/IORangeAllocator.h>
#include <IOKit/IODeviceTreeSupport.h>
#include <IOKit/IOPlatformExpert.h>
#include <IOKit/IOMapper.h>
#include <IOKit/IOLib.h>
#include <IOKit/assert.h>

#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/pci/IOPCIBridge.h>

/* events the user application reading /dev/sonypi can use */

#define SONYPI_EVENT_JOGDIAL_DOWN		 1
#define SONYPI_EVENT_JOGDIAL_UP			 2
#define SONYPI_EVENT_JOGDIAL_DOWN_PRESSED	 3
#define SONYPI_EVENT_JOGDIAL_UP_PRESSED		 4
#define SONYPI_EVENT_JOGDIAL_PRESSED		 5
#define SONYPI_EVENT_JOGDIAL_RELEASED		 6	/* obsolete */
#define SONYPI_EVENT_CAPTURE_PRESSED		 7
#define SONYPI_EVENT_CAPTURE_RELEASED		 8	/* obsolete */
#define SONYPI_EVENT_CAPTURE_PARTIALPRESSED	 9
#define SONYPI_EVENT_CAPTURE_PARTIALRELEASED	10
#define SONYPI_EVENT_FNKEY_ESC			11
#define SONYPI_EVENT_FNKEY_F1			12
#define SONYPI_EVENT_FNKEY_F2			13
#define SONYPI_EVENT_FNKEY_F3			14
#define SONYPI_EVENT_FNKEY_F4			15
#define SONYPI_EVENT_FNKEY_F5			16
#define SONYPI_EVENT_FNKEY_F6			17
#define SONYPI_EVENT_FNKEY_F7			18
#define SONYPI_EVENT_FNKEY_F8			19
#define SONYPI_EVENT_FNKEY_F9			20
#define SONYPI_EVENT_FNKEY_F10			21
#define SONYPI_EVENT_FNKEY_F11			22
#define SONYPI_EVENT_FNKEY_F12			23
#define SONYPI_EVENT_FNKEY_1			24
#define SONYPI_EVENT_FNKEY_2			25
#define SONYPI_EVENT_FNKEY_D			26
#define SONYPI_EVENT_FNKEY_E			27
#define SONYPI_EVENT_FNKEY_F			28
#define SONYPI_EVENT_FNKEY_S			29
#define SONYPI_EVENT_FNKEY_B			30
#define SONYPI_EVENT_BLUETOOTH_PRESSED		31
#define SONYPI_EVENT_PKEY_P1			32
#define SONYPI_EVENT_PKEY_P2			33
#define SONYPI_EVENT_PKEY_P3			34
#define SONYPI_EVENT_BACK_PRESSED		35
#define SONYPI_EVENT_LID_CLOSED			36
#define SONYPI_EVENT_LID_OPENED			37
#define SONYPI_EVENT_BLUETOOTH_ON		38
#define SONYPI_EVENT_BLUETOOTH_OFF		39
#define SONYPI_EVENT_HELP_PRESSED		40
#define SONYPI_EVENT_FNKEY_ONLY			41
#define SONYPI_EVENT_JOGDIAL_FAST_DOWN		42
#define SONYPI_EVENT_JOGDIAL_FAST_UP		43
#define SONYPI_EVENT_JOGDIAL_FAST_DOWN_PRESSED	44
#define SONYPI_EVENT_JOGDIAL_FAST_UP_PRESSED	45
#define SONYPI_EVENT_JOGDIAL_VFAST_DOWN		46
#define SONYPI_EVENT_JOGDIAL_VFAST_UP		47
#define SONYPI_EVENT_JOGDIAL_VFAST_DOWN_PRESSED	48
#define SONYPI_EVENT_JOGDIAL_VFAST_UP_PRESSED	49
#define SONYPI_EVENT_ZOOM_PRESSED		50
#define SONYPI_EVENT_THUMBPHRASE_PRESSED	51
#define SONYPI_EVENT_MEYE_FACE			52
#define SONYPI_EVENT_MEYE_OPPOSITE		53
#define SONYPI_EVENT_MEMORYSTICK_INSERT		54
#define SONYPI_EVENT_MEMORYSTICK_EJECT		55
#define SONYPI_EVENT_ANYBUTTON_RELEASED		56
#define SONYPI_EVENT_BATTERY_INSERT		57
#define SONYPI_EVENT_BATTERY_REMOVE		58
#define SONYPI_EVENT_FNKEY_RELEASED		59
#define SONYPI_EVENT_WIRELESS_ON		60
#define SONYPI_EVENT_WIRELESS_OFF		61
#define SONYPI_EVENT_ZOOM_IN_PRESSED		62
#define SONYPI_EVENT_ZOOM_OUT_PRESSED		63

/* get/set brightness */
#define SONYPI_IOCGBRT		_IOR('v', 0, __u8)
#define SONYPI_IOCSBRT		_IOW('v', 0, __u8)

/* get battery full capacity/remaining capacity */
#define SONYPI_IOCGBAT1CAP	_IOR('v', 2, __u16)
#define SONYPI_IOCGBAT1REM	_IOR('v', 3, __u16)
#define SONYPI_IOCGBAT2CAP	_IOR('v', 4, __u16)
#define SONYPI_IOCGBAT2REM	_IOR('v', 5, __u16)

/* get battery flags: battery1/battery2/ac adapter present */
#define SONYPI_BFLAGS_B1	0x01
#define SONYPI_BFLAGS_B2	0x02
#define SONYPI_BFLAGS_AC	0x04
#define SONYPI_IOCGBATFLAGS	_IOR('v', 7, __u8)

/* get/set bluetooth subsystem state on/off */
#define SONYPI_IOCGBLUE		_IOR('v', 8, __u8)
#define SONYPI_IOCSBLUE		_IOW('v', 9, __u8)

/* get/set fan state on/off */
#define SONYPI_IOCGFAN		_IOR('v', 10, __u8)
#define SONYPI_IOCSFAN		_IOW('v', 11, __u8)

/* get temperature (C) */
#define SONYPI_IOCGTEMP		_IOR('v', 12, __u8)













#define SONYPI_DEVICE_MODEL_TYPE1	1
#define SONYPI_DEVICE_MODEL_TYPE2	2
#define SONYPI_DEVICE_MODEL_TYPE3	3

/* type1 models use those */
#define SONYPI_IRQ_PORT			0x8034
#define SONYPI_IRQ_SHIFT		22
#define SONYPI_TYPE1_BASE		0x50
#define SONYPI_G10A			(SONYPI_TYPE1_BASE+0x14)
#define SONYPI_TYPE1_REGION_SIZE	0x08
#define SONYPI_TYPE1_EVTYPE_OFFSET	0x04

/* type2 series specifics */
#define SONYPI_SIRQ			0x9b
#define SONYPI_SLOB			0x9c
#define SONYPI_SHIB			0x9d
#define SONYPI_TYPE2_REGION_SIZE	0x20
#define SONYPI_TYPE2_EVTYPE_OFFSET	0x12

/* type3 series specifics */
#define SONYPI_TYPE3_BASE		0x40
#define SONYPI_TYPE3_GID2		(SONYPI_TYPE3_BASE+0x48) /* 16 bits */
#define SONYPI_TYPE3_MISC		(SONYPI_TYPE3_BASE+0x6d) /* 8 bits  */
#define SONYPI_TYPE3_REGION_SIZE	0x20
#define SONYPI_TYPE3_EVTYPE_OFFSET	0x12

/* battery / brightness addresses */
#define SONYPI_BAT_FLAGS	0x81
#define SONYPI_LCD_LIGHT	0x96
#define SONYPI_BAT1_PCTRM	0xa0
#define SONYPI_BAT1_LEFT	0xa2
#define SONYPI_BAT1_MAXRT	0xa4
#define SONYPI_BAT2_PCTRM	0xa8
#define SONYPI_BAT2_LEFT	0xaa
#define SONYPI_BAT2_MAXRT	0xac
#define SONYPI_BAT1_MAXTK	0xb0
#define SONYPI_BAT1_FULL	0xb2
#define SONYPI_BAT2_MAXTK	0xb8
#define SONYPI_BAT2_FULL	0xba

/* FAN0 information (reverse engineered from ACPI tables) */
#define SONYPI_FAN0_STATUS	0x93
#define SONYPI_TEMP_STATUS	0xC1

/* ioports used for brightness and type2 events */
#define SONYPI_DATA_IOPORT	0x62
#define SONYPI_CST_IOPORT	0x66

#define SONYPI_CAMERA_BRIGHTNESS		0
#define SONYPI_CAMERA_CONTRAST			1
#define SONYPI_CAMERA_HUE			2
#define SONYPI_CAMERA_COLOR			3
#define SONYPI_CAMERA_SHARPNESS			4

#define SONYPI_CAMERA_PICTURE			5
#define SONYPI_CAMERA_EXPOSURE_MASK		0xC
#define SONYPI_CAMERA_WHITE_BALANCE_MASK	0x3
#define SONYPI_CAMERA_PICTURE_MODE_MASK		0x30
#define SONYPI_CAMERA_MUTE_MASK			0x40

/* the rest don't need a loop until not 0xff */
#define SONYPI_CAMERA_AGC			6
#define SONYPI_CAMERA_AGC_MASK			0x30
#define SONYPI_CAMERA_SHUTTER_MASK 		0x7

#define SONYPI_CAMERA_SHUTDOWN_REQUEST		7
#define SONYPI_CAMERA_CONTROL			0x10

#define SONYPI_CAMERA_STATUS 			7
#define SONYPI_CAMERA_STATUS_READY 		0x2
#define SONYPI_CAMERA_STATUS_POSITION		0x4

#define SONYPI_DIRECTION_BACKWARDS 		0x4

#define SONYPI_CAMERA_REVISION 			8
#define SONYPI_CAMERA_ROMVERSION 		9

/* Event masks */
#define SONYPI_JOGGER_MASK			0x00000001
#define SONYPI_CAPTURE_MASK			0x00000002
#define SONYPI_FNKEY_MASK			0x00000004
#define SONYPI_BLUETOOTH_MASK			0x00000008
#define SONYPI_PKEY_MASK			0x00000010
#define SONYPI_BACK_MASK			0x00000020
#define SONYPI_HELP_MASK			0x00000040
#define SONYPI_LID_MASK				0x00000080
#define SONYPI_ZOOM_MASK			0x00000100
#define SONYPI_THUMBPHRASE_MASK			0x00000200
#define SONYPI_MEYE_MASK			0x00000400
#define SONYPI_MEMORYSTICK_MASK			0x00000800
#define SONYPI_BATTERY_MASK			0x00001000
#define SONYPI_WIRELESS_MASK			0x00002000


/* The set of possible ioports */
struct sonypi_ioport_list {
	u16	port1;
	u16	port2;
};

/* The set of possible interrupts */
struct sonypi_irq_list {
	u16	irq;
	u16	bits;
};

struct sonypi_event {
	u8	data;
	u8	event;
};


struct sonypi_eventtypes {
	int			model;
	u8			data;
	unsigned long		mask;
	struct sonypi_event *	events;
};

#define SONYPI_BUF_SIZE	128

/* Correspondance table between sonypi events and input layer events */
static struct sonypi_input_ev{
	int sonypiev;
	int inputev;
};

struct sonypi_keypress {
	struct input_dev *dev;
	int key;
};

class SonyPiClass : public IOPCIDevice
{
    OSDeclareDefaultStructors(SonyPiClass)

protected:
    IOSimpleLock *		lock;
    IODeviceMemory *		ioMemory;
    IOMemoryMap *		configAddrMap;
    IOMemoryMap *		configDataMap;

    volatile UInt32	*	configAddr;
    volatile UInt8	*	configData;



public:
    virtual bool start(	IOService * provider );
    virtual bool configure( IOService * provider );
    virtual void free();

    virtual IODeviceMemory * ioDeviceMemory( void );

    virtual UInt32 configRead32( IOPCIAddressSpace space, UInt8 offset );
    virtual void configWrite32( IOPCIAddressSpace space,
					UInt8 offset, UInt32 data );
    virtual UInt16 configRead16( IOPCIAddressSpace space, UInt8 offset );
    virtual void configWrite16( IOPCIAddressSpace space,
					UInt8 offset, UInt16 data );
    virtual UInt8 configRead8( IOPCIAddressSpace space, UInt8 offset );
    virtual void configWrite8( IOPCIAddressSpace space,
					UInt8 offset, UInt8 data );

    virtual IOReturn setDevicePowerState( IOPCIDevice * device,
                                          unsigned long whatToDo );

};

#endif _IOKIT_SONY_PI_H
