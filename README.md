# rtop
**rtop** is a performance monitor designed for the Rockchip RK3566/68/88 processors.<br><br>

![Screenshot from 2024-08-06 10-19-24](https://github.com/user-attachments/assets/724767cc-bb50-4c77-871f-7d82368de8b9)
![Screenshot from 2024-08-07 13-11-48](https://github.com/user-attachments/assets/cedcd536-a620-4bba-82f3-24d2a68fe8e9)<br><br>

## rtop for Ubuntu OS by Joshua Riek.<br/>
[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)<br/><br/>
Specifically built for Rockchip processors using Qt5 by [Q-engineering](https://qengineering.eu/)

##### If you have the original Rock OS, please install https://github.com/Qengineering/rtop-KDE

------------

## Installing rtop.
To install **rtop**, download the repository, make the install script executable, and run it:<br/>
```script
$ git clone https://github.com/Qengineering/rtop-Ubuntu.git 
$ sudo chmod +x ./install_rtop.sh 
$ sudo ./install_rtop.sh 
```

------------

## Running rtop.
To run **rtop** from the command line in the terminal:<br/> 
```
$ rtop
```
Alternatively, you can use the shortcut available in your software overview.<br><br>
![Screenshot from 2024-08-06 10-32-32](https://github.com/user-attachments/assets/b6576f44-f138-41a3-b69a-6ab556d88928)<br><br>
You can also make **rtop** a favorite.<br><br>
![Screenshot from 2024-08-06 10-31-35](https://github.com/user-attachments/assets/acd56610-7ca8-4fee-ac82-a907e6d8b76e)

------------

## Permissions.
As noted in the appendix, some information requires sudo permissions.<br>
The installation script should handle this automatically.<br><br>
If you don't see RGA bars, it could indicate a permissions issue.<br>
To resolve this, try running **rtop** with sudo:<br>
```
$ sudo /usr/local/bin/rtop
```
Alternatively, you can disable the password prompt globally using `sudo visudo`.<br><br>
If the NPU bars are not visible, ensure the NPU is enabled by following the commands provided in the appendix.<br>

------------

## Qt5.
For your convenience, the Qt5 code is included.<br/> 

------------

### Appendix.
The used commands are:<br/> 
```
#CPU load
$ cat /proc/stat
#CPU freq
$ cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq (or cpu1, cpu2 etc)
#GPU load
$ cat /sys/class/devfreq/fb000000.gpu/load
#GPU freq
$ cat /sys/class/devfreq/fb000000.gpu/cur_freq
#NPU load
$ sudo cat /sys/kernel/debug/rknpu/load
#NPU freq
$ cat /sys/class/devfreq/fdab0000.npu/cur_freq
#RGA load
$ sudo cat /sys/kernel/debug/rkrga/load
#RGA freq
$ sudo cat /sys/kernel/debug/clk/clk_summary | grep rga
#MEM
auto memInfo = readMemInfo(); (C++)
#SWAP
auto memInfo = readMemInfo(); (C++)
#TEMP
$ cat /sys/class/thermal/thermal_zone0/temp
#FAN
$ cat /sys/class/thermal/cooling_device4/cur_state
#DEVICE
$ cat /sys/firmware/devicetree/base/compatible

```

------------

[![paypal](https://qengineering.eu/images/TipJarSmall4.png)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CPZTM5BB3FCYL) 
