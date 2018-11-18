# Unraid VM Manager

This is a two part app. Code in the `arduino` folder is supposed to be compiled
and run on an Arduino. Code in the `server-app` is supposed to bu run on the
Unraid server in a docker container.

![](https://github.com/micke/unraid-vm-manager/blob/master/docs/1.jpg)
![](https://github.com/micke/unraid-vm-manager/blob/master/docs/2.jpg)

1. Install the `Virtual Machine Wake On Lan` plugin on your unraid server.  
  __Note__: You might have to enable the plugin in the VM settings page in
  Unraid. Turn onCheck "Advanced" mode.
2. Edit `arduino/config.h` and compile and run on a D1 mini.  
  `uid` is the ID the VM has on your Unraid server.  
  `pin` is the pin where you have the switch for the VM.  
  `activityPin` is the pin that you connect to a LED that shows the status of the virtual machine.
  You'll have to figure out how the pins should be hooked up and configured.  
  I'm sorry but I currently don't have time to draw a schematic.
3. Install the accompanying docker container on your Unraid server.
  ![](https://github.com/micke/unraid-vm-manager/blob/master/docs/container.png)
4. __BONUS!__ The design for the housing can be found in the lasercut.pdf file.  
  Use a lasercutting service to cut your own little box to house the
   controller in. I used https://cotter.dk

## [Notes by /u/RoadrageWorker on reddit](https://www.reddit.com/r/unRAID/comments/9wgh5m/just_open_sourced_physical_vm_controller_using_d1/e9qlvxm)
For all like me that are new-ish to Arduino, some advice to get this project rolling:

* When using the D1 mini (and that's what it is made for), you need the board manager files for the Arduino IDE, insert http://arduino.esp8266.com/stable/package_esp8266com_index.json into the "Additional Board Manager URL" field.
* Then load the files via "Tools > Board", look for esp8266 (I am on a different machine right now without the IDE, so the labels might be a bit off).
* Select this board afterwards: "LOLIN(WEMOS)D1 R2 & mini"

Additionally, you will need two libraries that can be found on github and are required by the project.

* https://github.com/koenieee/WakeOnLan-ESP8266
* https://github.com/JoaoLopesF/RemoteDebug
* (unzip and drop into the libraries folder)

When editing the config file, you only need to edit the first 4 lines of each VM, they are as follows:

* MAC (has to be unique)
* UUID
* switch pin
* LED pin

MAC and UUID can be found in the unRAID VM manager, advanced view

This is a sample layout for two VMs including a safety for the destroy function by using two buttons:
![](https://github.com/micke/unraid-vm-manager/blob/master/docs/schematic.png)
