# Unraid VM Manager

This is a two part app. Code in the `arduino` folder is supposed to be compiled
and run on an Arduino. Code in the `server-app` is supposed to bu run on the
Unraid server in a docker container.

1. Install the `Virtual Machine Wake On Lan` plugin on your unraid server.  
  __Note__: You might have to enable the plugin in the VM settings page in
  Unraid. Turn on "Advanced" mode.
2. Edit `arduino/config.h` and compile and run on a D1 mini.  
  ID is the ID the VM has on your Unraid server.  
  You'll have to figure out how the pins should be hooked up and configured.  
  I'm sorry but I currently don't have time to draw a schematic.
3. Install the accompanying docker container on your Unraid server.
  ![](https://github.com/micke/unraid-vm-manager/blob/master/docs/container.png)
