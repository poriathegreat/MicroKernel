# MicroKernel
### A light kernel like system for low-end microcontrollers. Optimized for the W25Q64 flash and STM32f103.

## Description
### A light kernel to unlock baremetal multitasking on low end microcontrollers.
Using the system_setup.h file, a baremetal system with multitasking and most initial features 
will be setup in the matter of minutes. 

A light SYSTEM will usually follow the pattern of having:
1. Debugging
2. SerialNumber
3. Status LED
4. Standby functionality
5. Initial PIN configuration
6. Non volotile memory controller
7. Battery controller
8. A main data buffer
9. Encryption for the data buffer
10. UUID check (for security)


Then you have modules, sensors and signals connected to the system.
The system will want to interact with these other parts (read/write).
This is where the INTERFACE layer comes in.
1. The sensors collecting data
2. The modules giving status
3. The signals, trying to tell the system something.
4. The functions to pass things to these modules/sensors.
5. The functions to set the I/O ports/DAC/PWM etc..

A pointer to the data related to a library is added to the interfaceDataStructure.
The pointer is then initialized within the interface_firstInit function.

In this layer we INTERFACE different aspects into the system, and 
we create INTERACTION POINTS for the next layer (the TASKS) to control.


And then we have the multitasking features (the TASKS):
1. The Non blocking Tasks (tasks_main)
2. The Initialization of those Tasks (tasks_firstInit)
3. Timers controlling timing of those tasks. (tasks_tick)


Last but not least, once all the data is gathered and processed, on smaller devices this data is usually
sent elsewhere. That where we have the PassData layer, which will read data from the buffer
and pass it to the next layer. (Maybe send with RF, BLE, GSM, CAN. RS485...)

This project structure has been setup in this kernel to allow rapid structured system development.
This allows us to create extremely modular systems, and include them accross different projects with ease.
Working on microcontrollers, moving around different systems is the biggest challenge.


The Goal:
Working on baremetal systems has one major downside, there is no standard for coding.
Anyone working in the industry has their own coding style. This makes maintaining modular systems rather hard. Even in small teams.
RTOS is a great solution, but not on low end, 64KB flash and 20KB RAM, microcontrollers.
That's where this system comes in.

The Problem:
Different libraries, even the ones used in a single system, usually have different structures.
That is because anyone has their own coding style. This dramatically impacts readability of the code. 
I've even seen teams rewriting code that works just to improve readability. 
And then, there's the problem of using DELAY in code. 
Much of the baremetal code available to everyone is based on DELAY, (probably since its the first thing everyones learns with that blinking LED project!)
which is a very inefficient blocking method. 
And theeeeen, we have RTOS. A very nice structures system with clean time management. Even let's you use the osDealy() function with ease, no worries about blocking anything.
However, on smaller RAM/FLASH, you can only fit so little. 

The Solution:
A light weight system, with the necessary features needed to quickly get started.
This system would need four main parts:
1. The system its self
2. The "Interfaces", system aspects that collect data and provide interaction with outside of the MCU.
3. The "Tasks", processes running at all time, reading and writing data collected and produced by the interfaces.
4. The passData layer, the data processed and ready to be passed to the master system (a server, ble connection, RS485 port, CAN bus, ...)

The system would handle the timing, the watchdog, a status led, battery checking, debugging, etc...
The interface would handle data collection of different aspects added to the system, their initialization, their data storage and handling, etc....
The tasks would processe the data collected in the structures of each senosr/aspect of the system, and create signals based on it.
The signals either immediately take effect on the system (change I/O, show something on LCD, shutoff the system, restart a module, ...) or they add something to the buffer to be processed futher down the line (sent to a server/ ble/ RS485 ...).

## The Provided Structure:
### This kernel follows the same simple structure for each and every part the system explained below. 
In this structure, each task should have the following functions:

    t_theExampleModule_init();
This function will hold anything needed for the initialization of am aspect. 
This function will be put in the interface_firstInit() function of the interface/int_firstInit.c file.
This function will then be called once upon initialization of the system.

    t_theExampleModule_main();
This function will handle the module. It will be called all the time by the superloop. 
This function will be put in the interface_main() function of the interface/int_main.c file.
This function MUST be non blocking! As it will effect the overall performance of the system.

    t_theExampleModule_tick();
This function will be responsible for the timers of the system aspect, and will be called every 1ms.
This function should be filled with counters, which will be called every 1ms to count up.
In the theExampleModule_main() these counters (timers) can be checked to see whether its time to run the 
main or wait. 
This function will be put in the interface_tick() function of the interface/int_clock.c file.

    typedef struct{
      _Bool    theExampleModuleData1;
      int      theExampleModuleData2;
      uint8_t  theExampleModuleData3;
    }theExampleModule1DataStructure;
    extern theExampleModule1DataStructure theExampleModule1Data;
This structure will hold the Module's public data. It will be available to anyone who has this module included.
That being said, this data is available to interface and every task, however it is not (and should not) be available to other aspects (sensors/modules/..etc), allowing us to switch between systems easily.
Modules and sensors will not depend on each other for the code to work, but the task using the two sensors/modules will stop working, which is fine.

so the interface structure would look like this:

    typedef struct{
        theExampleModule1DataStructure*       dataof1;
        theExampleModule2DataStructure*       dataof2;    
    }interfaceStructure;


and in the interface_firstInit() would need this (which will be called once when the system startsup):

	interfaceData.dataof1 = &theExampleModule1Data;
	interfaceData.dataof2 = &theExampleModule2Data;
So in different tasks, if we were to access this module's data, we would access it like this:

    parameter1 = interface.dataof1->theExampleModuleData1;
    parameter2 = interface.dataof1->theExampleModuleData2;

and that of module2:

    module2Param1 = interface.dataof2->theExampleModuleData1;
    module2Param2 = interface.dataof2->theExampleModuleData2;

and so on with every aspect of the system.

# Design Details
### The details on how everything works, this part should provide a complete understanding of the kernel layers.
## The Kernel Overview
```mermaid
mindmap
root(sys_setup.h is where you need to start. This will configure the system addons automatically.)
    systemAddons(By editing sys_setup.h file, the sys_addons.h file now has a few libraries that are included. We need to edit those libraries.)
        (We add the modules, sensors, etc. to the interface folder. Each module library should be placed in interface/LIB/libraryFolder)
        (The data collected by INTERFACE is proccessed here by different "tasks".)
````

## The System Layer
### The System Overview

```mermaid

mindmap
        (Essential features are configured and added to the System Layer)
            (Checking the battery voltage)
            (Configuring I/O upon start)
            (Loading a serial number from internal/external memory)
            (Loading/Saving device status to internal/external memory)
            (Enterning standby mode, for low power applications)
            (Showing multiple status of device, using a single LED)
            (Having a Multiple Input Single Output buffer. Processing all the data and passing it to a single Master)
            (Having a light and practical console interface)
            (Being able to lock the code written to the MCU UUID)
            (Having an internal, light, yet effective encryption stack)
            (Having a nice structure for higher layer code to be written onto)
````

### The System Flow
```mermaid

---
title: The System Layer
---
classDiagram

    class system_setup["sys_setup.h"]
    class system_setup{
        #define SYS_PROJECT_NAME
        #define SYS_VERSION
        #define INIT_LOAD_SERIAL_FROM_MEMORY
        #define INIT_SYS_REQUEST_CONSOLE_DEBUG
        #define INIT_SYS_STAT_LED
        #define INIT_STANDBY_FUNCTIONALITY
        #define INIT_CONFIGURE_PIN_STATE
        #define INIT_SAVE_TO_MEMORY
        #define INIT_SYS_HAS_BATTERY
        #define INIT_SYS_USE_ENCYPTION
        #define INIT_SYS_BUFFER
        #define INIT_SYS_LOCK_UUID
        Result: sys_addon.h(autogenerated)
    }
    note "Start by editing
Inc_Prj/system_setup.h file.
This will generate the
system related files only
giving essentials features.
Anything turned off will
not be compiled thus
taking no space."

    class sys_addon["sys_addons.h"]
    class sys_addon{
        #include "sys_battery.h"
        #include "sys_standby.h"
        #include "sys_configure_io.h"
        #include "sys_savetomemory.h"
        #include "sys_statusled.h"
        #include "sys_loadserialnumber.h"
        #include "sys_buffer.h"
    }

    class sys_statusled{
        #define _STAT_LED_GPIO
        #define _STAT_LED_Pin
            *************
        structure s_statLed_struct[
        ---Bool 		anti;
        ---uint8_t 	deviceState;
        ]
        void s_statusLed_main(void);
        void s_statLed_tick(void);
    }

    class sys_battery{
        #define BATTERY_READ_FROM_ADC
        ----#define BATTERYADC_HANDLE
            *************
        structure s_batteryStructure[
        ---uint16_t voltage;
        ---uint8_t percentage;
        ]
        void s_battery_init(void);
        void s_battery_main(void);
        void s_battery_tick(void);
    }
    class sys_savetomemory{
        #define INIT_SAVE_EXTERNAL_MEMORY
        ----#define INTERNALMEMORY_ENDADRESS
            *************
        structure s_ROMdataStruct[
        ---Bool forceSave;
        ]
        void s_ROM_init(void);
        void s_ROM_main(void);
        void s_ROM_tick(void);
	void s_ROM_saveNow(void);
    }
    class sys_standby{
        void s_enterStandbyNow(void);
        syswakeup_t s_checkSleep(void);
        syswakeup_t s_checkWakeup(void);
    }
    class sys_configure_io{
        void s_configureio_init(void);
    }

    system_setup --> sys_statusled : Must be Edited
    system_setup --> sys_standby : Must be Edited
    system_setup --> sys_configure_io : Must be Edited
    system_setup --> sys_savetomemory : Must be Edited
    system_setup --> sys_battery : Must be Edited

    sys_statusled --> sys_addon
    sys_standby --> sys_addon
    sys_configure_io --> sys_addon
    sys_savetomemory --> sys_addon
    sys_battery --> sys_addon


    class sys_buffer["sys_buffer"]
    class sys_buffer{
        #define BUFFER_DATA_ENABLED
        #define CHECK_BUFFER_INTERVAL_MS
        #define CHECK_BUFFER_WAIT_TIME
        #define BUFFER_USE_ENCRYPTION
        #define BUFFER_USE_EXTERNAL_FLASH
        ----#define BUFFER_CELLS_IN_RAM_IF_NO_FLASH
            *************
        structure bufferDataStructure_t[
        ---Bool dataWaiting;
        ---Bool encryptionEnabled;
        ]
        void s_buffer_init(void);
        void s_buffer_main(void);
        void s_buffer_tick(void);
		buffer_addToBuffer(uint8_t *data, size_t dataSize);
		buffer_passDataToNextLayer(uint8_t *data, size_t dataSize);
    }

    class sys_debug["sys_debug"]
	class sys_debug{
		#define DEBUG_UART_HANDLE	huart1
		#define DEBUGGING
		uint8_t console_requestSpace(uint8_t neededSpace, char* title);
		void console_printf(uint8_t line, uint8_t part, const char *fmt, ...);
		void console_clearSection(uint8_t line, uint8_t part);
		void console_clearLine(uint8_t line);
		void debugprintf(const char *fmt, ...);
	}


    class sys_loadserialnumber["sys_loadserialnumber"]
	class sys_loadserialnumber{
		void s_loadSerialNumber(void);
	}



    sys_buffer --|> sys
    sys_loadserialnumber --|> sys
    sys_addon --|> sys
    sys_debug --|> sys



    class sys{
        structure sysInfoStructure_t[
        	--char                     serialNumber[16];
        	--char                     version[10];
            --Bool                     keepOn;
        	--uint8_t                  uuid[12];
        	--s_ROMdataStruct*         rom;
        	--s_statLed_struct*        ledStat;
        	--bufferDataStructure_t*   buffer;
        ]
        void sys_firstInit(void);
        void sys_main(void);
    }
sys .. Note_SystemReady
class Note_SystemReady{
By this point we 
have the system ready.
The sys_firstInit needs
to be placed before the
superloop of main.c
and sys_main needs to
be placed within the
super loop.
}
````

## The Interface Layer
### The Interface Overview
The Interface Layer will collect data from different aspects of the system and it will provide access points for the tasks layer to be able to interact with the rest of the system through the Interface Layer.
```mermaid
mindmap
        (The Interface Layer will collect data from every aspect of the system)
            (Temperature Sensor)
            (Humidity Sensor)
            (Proximity Sensor)
            (BLE Module)
            (GSM Module)
            (ESP WiFi)
            (CAN BUS)
            (RS485 BUS)
````
### The Interface Flow
The Interface Layer is called by the system.
Every aspect of the system will be imported into the Interface Layer.
Raw data is collected in the InterfaceDataStructure.
This structure will be available to tasks to process further.
In this layer we will try not to process data and keep the data as RAW as possible, and keep the data processing for the 
tasks layer.
```mermaid
---
title: The Interface Layer
---
classDiagram
class sys{
        structure sysInfoStructure_t[
        	--char                     serialNumber[16];
        	--char                     version[10];
            	--Bool                     keepOn;
        	--uint8_t                  uuid[12];
        	--s_ROMdataStruct*         rom;
        	--s_statLed_struct*        ledStat;
        	--bufferDataStructure_t*   buffer;
        ]
        void sys_firstInit(void);
        void sys_main(void);
    }



class interface{
    interfaceDataStructure interfaceData;
    interface_init()
    interface_main()
    interface_tick()
}

sys --|> interface : System will call the interface layer

class interface_main{
We will add the library_main functions
to this function. Interface_main
function will continuously call the
functions within.
tasks_main will be called from here,
calling all the tasks once, then
checking in on modules. 
    tasks_main();
 
    module1_main()
    module2_main()
    sensor1_main()
    sensor2_main()
}

class interface_init{
We will add the library_init functions
to this function. Interface_init
function will be called once when
the system starts.
tasks_init will be called from here.
    tasks_init();

    module1_init()
    module2_init()
    sensor1_init()
    sensor2_init()
}

class interface_tick{
We will add the library_tick functions
to this function. Interface_tick
function will be called every 1ms.
tasks_tick will be called from
here, calling every task timer once
every 1ms.
    tasks_tick();
    module1_tick()
    module2_tick()
    sensor1_tick()
    sensor2_tick()
}


class interfaceDataStructure{
A pointer to the dataStructure of
each system aspect should be added
to interface.
*********************************


    structure interfaceDataStructure[
    sysInfoStructure_t* sys;
    module1Structure*   module1;
    module2Structure*   module2;
    sensor1Structure*   sensor1;
    sensor2Structure    sensor2;
]

}

interface .. interface_main
interface .. interface_tick
interface .. interface_init
interface .. interfaceDataStructure
````

### The Interface Structure
Interfaces are placed in the Core/Src/Src_Prj/Interface/LIB folder.
Every system aspect will have its own .c/.h file inside its own folder. This will help us organize different interfaces.
In the INterface folder there will be 3 files: interface_main, interface_init, interface_tick. The corresponding function for each interface must be placed in these functions.
The dataStructure pointer must be initialized in the interface_init function where the module/sensor is initialized. 
```mermaid
---
title: The Interface Layer
---
classDiagram
class interface_main{
	sensor1_main();
	module1_main();
}
class interface_init{
	sensor1_init();
	interfaceData.sensor1 = &sensor1Data;
	module1_init();
	interfaceData.module1 = &module1Data;

}
class interface_tick{
	sensor1_tick();
	module1_tick();
}


class module1_main{
	module1_function1();
	module1_function2();
	module1_function3();
}
class module1_init{
	module1_initFunction1();
	module1_initfunction2();
}
class module1_tick{
	module1_counter1++;
	module1_counter2++;
	module1_counter3++;
	module1_counter4++;
}

class sensor1_main{
	senosr1_function1();
	sensor1_function2();
	sensor1_function3();
}
class sensor1_init{
	sensor1_initFunction1();
	sensor1_initfunction2();
}
class sensor1_tick{
	sensor1_counter1++;
	sensor1_counter2++;
	sensor1_counter3++;
	sensor1_counter4++;
}

class module1DataStructure{
	_Bool data1;
	_Bool switch1;
	int32 value1;
}
class sensor1DataStructure{
	_Bool data1;
	_Bool switch1;
	int32 value1;
}


class interfaceDataStructure{
	sensor1DataStructure* sensor1;
	module1DataStructure* module1;
}

interface_main <|-- sensor1_main
interface_tick <|-- sensor1_tick
interface_init <|-- sensor1_init
interfaceDataStructure <|-- module1DataStructure

interface_main <|-- module1_main
interface_tick <|-- module1_tick
interface_init <|-- module1_init
interfaceDataStructure <|-- sensor1DataStructure
````



## The Tasks Layer
### The Tasks Overview
The Tasks layer is responsible for processing data collected by the Interface layer.
The Interface Layer will also provide access points for the Tasks Layer to Interact with different
aspects of the system.
```mermaid
mindmap
        (The Tasks Layer will process data collected from different aspects of the system)
            (Check Temperature for more than 70C)
            (Process data received on RS485)
            (Refresh the LCD)
            (Check GPS position for change)
            (Check Humidity and sound the ALARM if more than 80%)
            (Check MOTION SENSOR for movement in Z-axes)
            (Process data received on CAN BUS)
            (Check hearbeat for less than 50bpm)
````

### The Tasks Flow
The Tasks Layer will have access to the System and to every Interface imported.
This data is available for different tasks to use.
The System will call the tasks layer as shown below.
```mermaid
---
title: The Tasks Layer
---
classDiagram
class sys{
        structure sysInfoStructure_t[
        	--char                     serialNumber[16];
        	--char                     version[10];
            	--Bool                     keepOn;
        	--uint8_t                  uuid[12];
        	--s_ROMdataStruct*         rom;
        	--s_statLed_struct*        ledStat;
        	--bufferDataStructure_t*   buffer;
        ]
        void sys_firstInit(void);
        void sys_main(void);
    }

class interfaceDataStructure{
structure interfaceDataStructure[
    sysInfoStructure_t* sys;
    module1Structure*   module1;
    module2Structure*   module2;
    sensor1Structure*   sensor1;
    sensor2Structure    sensor2;
]

}

class tasks{
    tasks_init()
    tasks_main()
    tasks_tick()
}

sys <|-- tasks : System will call the tasks layer
interfaceDataStructure --|> tasks : Tasks will have access to the interfaceDataStructure

class tasks_main{
We will add the task_main functions
to this function. Tasks_main
function will continuously call the
functions within.
 
    task1_main()
    task2_main()
    task3_main()
    task4_main()
}

class tasks_init{
We will add the task_init functions
to this function. Task_init
function will be called once when
the system starts.
tasks_init will be called from here.
    task1_init()
    task2_init()
    task3_init()
    task4_init()
}

class tasks_tick{
We will add thetask_tick functions
to this function. Task_tick
function will be called every 1ms.
    task1_tick()
    task2_tick()
    task3_tick()
    task4_tick()
}


tasks .. tasks_main
tasks .. tasks_tick
tasks .. tasks_init

````
### The Tasks Structure
Tasks are placed in the Core/Src/Src_Prj/Tasks/LIB folder.
Every task will have its own .c/.h file inside its own folder. This will help us organize different tasks.
In the Tasks folder there will be 3 files: tasks_main, tasks_init, tasks_tick. The corresponding function for each task must be placed in these functions.

```mermaid
---
title: The Tasks Layer
---
classDiagram
class tasks_main{
	task1_main();
	task2_main();
}
class tasks_init{
	task1_init();
	task2_init();
}
class tasks_tick{
	task1_tick();
	task2_tick();
}


class task1_main{
	task1_function1();
	task1_function2();
	task1_function3();
}
class task1_init{
	task1_initFunction1();
	task1_initfunction2();
}
class task1_tick{
	task1_counter1++;
	task1_counter2++;
	task1_counter3++;
	task1_counter4++;
}

class task2_main{
	task2_function1();
	task2_function2();
	task2_function3();
}
class task2_init{
	task2_initFunction1();
	task2_initfunction2();
}
class task2_tick{
	task2_counter1++;
	task2_counter2++;
	task2_counter3++;
	task2_counter4++;
}


tasks_main <|-- task1_main
tasks_tick <|-- task1_tick
tasks_init <|-- task1_init

tasks_main <|-- task2_main
tasks_tick <|-- task2_tick
tasks_init <|-- task2_init

````

















