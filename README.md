# Tiny Tapeout DIP firmware

The TT DIP is a breakout for Chip on Board (CoB) ASICs that let's you drop in the ASIC to a breadboard or circuit.

![DIP](images/TTDIP.png)

It offers 2 modes:

  1) Passive breakout for Tiny Tapeout Multi-project ASIC; and
  2) Auto-configured to act as a breakout for a selected project
  
![DIP Pinout](images/tt-dip-pinout.png)
  
## Multi-project ASIC

In this mode, *you* get to control the ASIC MUX and select the project, by controlling the multiplexer pins.

In addition, all the I/O, clock and reset and available to you to interact with the project.

## Auto-configured

You may also set things up to automatically select a particular project on the ASIC, optionally clocking it from the on-board MCU.  In this way, the breakout acts as a specific project right from power-up.

### TUI

A serial port is available through the USB-C connector.

![TUI](images/TUI.png)

This allows you to select designs, set the clock rate, etc.

### Commander

In addition, this should allow control through the [Tiny Tapeout commander](https://commander.tinytapeout.com/) interface, right in the browser.

![Commander](images/commander.png)


# New firmware to auto-configure

To create and burn new firmware:

 1) ensure you have submodules, doing `git submodule update --init --recursive`
 
 2) you may want to edit [projectselect.h](src/projectselect.h) to set `TT_DIP_DEFAULT_DESIGN_ID` (default design to load) and `TT_DIP_DEFAULT_CLOCK_HZ`
 
 3) **hold** the BOOT button as prior to and during insertion into USB port and release once inserted
 
 4) From the src directory, issue `make` which will both build and burn the new firmware
 
You should see something like
![Build and Burn](images/buildandburn.png)

This assumes your user can access the USB device that shows up.  Under Linux, I added a rule like

```
SUBSYSTEM=="usb", ATTR{idVendor}=="1a86", ATTR{idProduct}=="55e0", GROUP="plugdev", OWNER:="myuser", SYMLINK+="wchlink-%k", TAG+="uaccess"
```

Even so, there are occasions where the process fails to complete.  meh.  Recipe so far is just to unplug and try again.




