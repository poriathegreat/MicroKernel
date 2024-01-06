# MicroKernel
### A light kernel like system for low-end microcontrollers. Optimized for the W25Q64 flash and STM32f103.

## Description:
A light kernel to unlock baremetal multitasking on low end microcontrollers.
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

Then you have the multitasking features (the INTERFACE):
1. The Non blocking Tasks (interface_tasks)
2. The Initialization of those Tasks (interface_firstInit)
3. Timers controlling timing of those tasks. (interface_clock)
4. Structures holding the data of those tasks. (interface_fetchData)
5. A function to publicly share the needed data of those tasks. (interface_fetchData)
6. Last but not least, once all the data is gathered and processed, on smaller devices this data is usually
sent elsewhere. That where we have the interface_passData function, which will read data from the buffer
and pass it to the next layer. (Maybe send with RF, BLE, GSM...)

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
And theeen, we have RTOS. A very nice structures system with clean time management. Even let's you use the osDealy() function with ease, no worries about blocking anything.
However, on smaller RAM/FLASH, you can only fit so little. 

The Solution:
A light weight system, with the necessary features needed to quickly get started.
This system would need two main parts:
1. The system its self
2. The user added "tasks"

The system would handle the timing, the watchdog, a status led, battery checking, debugging, etc...
The interface would handle tasks, their initialization, their data storage and handling, etc...

The Provided Structure:
In this structure, each task should have the following functions:

    t_theExampleTask_init();
This function will hold anything needed for the initialization of the task. 
This function will be put in the interface_firstInit() function of the interface/int_firstInit.c file.
This function will then be called once upon initialization of the system.

    t_theExampleTask_main();
This function will handle the task. It will be called all the time by the superloop. 
This function will be put in the interface_tasks() function of the interface/int_tasks.c file.
This function MUST be non blocking! As it will effect the overall performance of the system.

    t_theExampleTask_tick();
This function will be responsible for the timers of the task, and will be called every 1ms.
This function should be filled with counters, which will be called every 1ms to count up.
In the theExampleTask_main() these counters (timers) can be checked to see whether its time to run the 
task or continue the task, etc. 
This function will be put in the interface_tick() function of the interface/int_clock.c file.

    typedef struct{
      _Bool    theExampleTaskData1;
      int      theExampleTaskData2;
      uint8_t  theExampleTaskData3;
    }theExampleTaskDataStructure;
    extern theExampleTaskDataStructure theExampleTaskData;
This structure will hold the task's public data. It will be available to anyone who has this task included.
That being said, this data is not available to other tasks, and other task libraries SHOULD NOT include this task's header
to see its data. This makes moving around different systems harder.
Instead, all the system wide public data is stored in one main structure sysData.
sysdata has a smaller counter part interface, and inside interface is all the data available for the entire system.

The interface_fetchData() function inside the interface/int_fetchData.c will be responsible for moving the needed data collected
by tasks inside their structure to the more public sysData.interface structure.

so the interface structure would look like this:

    typedef struct{
        theExampleTaskDataStructure       theExampleTask;
        theOtherExampleTaskDataStructure  theOtherExampleTask;
    
    
    
    }interfaceStructure;


and in the interface_fetchData() would need this (which will be called every FETCH_DATA_EVERY (default 10ms)):

    memcpy( &sysData.interface.theExampleTask, &theExampleTaskData, sizeof(theExampleTaskDataStructure) );
So in different tasks, if we were to access this task's data, we would access it like this:

    parameter = sysData.interface.theExampleTask.theExampleTaskData2;














