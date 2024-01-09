# Design Details
### The details on how everything works, this part should provide a complete understanding of the kernel layers.
## The Overview
```mermaid
mindmap
root(sys_setup.h is where you need to start. This will configure the system addons automatically.)
    systemAddons(By editing sys_setup.h file, the sys_addons.h file now has a few libraries that are included. We need to edit those libraries.)
        (We add the modules, sensors, etc. to the interface folder. Each module library should be placed in interface/LIB/libraryFolder)
        (The data collected by INTERFACE is proccessed here by different "tasks".)
````

```mermaid

---
title: Animal example
---
classDiagram
    note "From Duck till Zebra"
    Animal <|-- Duck
    note for Duck "can fly\ncan swim\ncan dive\ncan help in debugging"
    Animal <|-- Fish
    Animal <|-- Zebra
    Animal : +int age
    Animal : +String gender
    Animal: +isMammal()
    Animal: +mate()
    class Duck{
        +String beakColor
        +swim()
        +quack()
    }
    class Fish{
        -int sizeInFeet
        -canEat()
    }
    class Zebra{
        +bool is_wild
        +run()
    }

````
## The Interface Layer

```mermaid

---
title: The Interface Layer
---
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
## The Tasks Layer

```mermaid

---
title: The Tasks Layer
---
mindmap
        (The Tasks Layer will collect data from every aspect of the system)
            (Check Temperature for more than 70C)
            (Process data received on RS485)
            (Refresh the LCD)
            (Check GPS position for change)
            (Check Humidity and sound the ALARM if more than 80%)
            (Check MOTION SENSOR for movement in Z-axes)
            (Process data received on CAN BUS)
            (Check hearbeat for less than 50bpm)
````

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
## The System:


















