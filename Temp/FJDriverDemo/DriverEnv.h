#ifndef DRIVER_ENV_H
#define DRIVER_ENV_H

/******************************************************************************************************************
* VisualDDK(推荐)  -- http://visualddk.sysprogs.org/, 开源。
*   开发调试驱动的VS插件，提供了驱动项目的向导，可通过TCPIP在远程机器上安装驱动，支持VirtualBox/VMWare
*      
* DDKWizard -- http://ddkwizard.assarbad.net/

* Visual Studio 中工程向导中： General->Makefile Project，并设置对应的 Build command line 等
*
******************************************************************************************************************/


/******************************************************************************************************************
* 现在推荐使用WDK开发驱动，可以兼容WinNT ~ Win7 等各个系统
*
* VXD() -- Win9X平台的，虚拟设备驱动程序, Win2K后完全被淘汰
* 
* DDK(Driver Development Kit) -- 使用WDM(Windows Driver Model)驱动模型,集成了测试套件HCT？，适用于WinXP及之前的平台
*   NT类型 -- 不支持即插即用的NT式驱动?使用 NTDDK.h 头文件。不分层？
* WDM(Windows Driver Model) -- Vista以前的驱动模型。
*   支持即插即用,使用 WDM.h 头文件。建立在NT式驱动程序模型基础上，基于分层的，完成一个设备的操作，至少需要两个驱动设备共同完成。
*   一个PDO(物理设备对象，由总线驱动自动创建)，一个FDO(功能设备对象)并附加到PDO上。
*   其中还有可能存在过滤驱动。通过自定义设备扩展(如名为NextStackDevice)记录并找到下层的过滤驱动，通过 AttachedDevice 找到上层驱动
* 
* WDK(Windows Driver Kit) -- 使用WDF(Windows Driver Foundation)模型，简化WDM的开发，
*   适用于Vista及之后版本OS，包含了以前的DDK，也能编译Win2K等平台的驱动
*   以WDM为基础进行了建模和封装，降低了开发难度：基于对象（属性、方法、事件），封装了一些共同的默认行为（如即插即用和电源管理）
*   驱动分为内核模式(KMD框架)和用户模式(UMD框架)的两种驱动程序。    
* 
******************************************************************************************************************/


/******************************************************************************************************************
* SOURCES -- 其内容关系到该模块要编译那些文件，及编译出来的 .sys 文件的名字
******************************************************************************************************************/


#endif //DRIVER_ENV_H