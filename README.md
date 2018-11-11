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
