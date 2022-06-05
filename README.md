# M5-Stack-Geiger-Counter

This is a geiger counter Built on M5Stack

## How to use

Connect to M5STACK_SETUP wifi network <br>
Go to 192.168.4.1 on your device <br>
And Voila
 
For now it counts the A button presses

### By using, downloading, adapting in any shape or form this project, you agree to [CC By Sa](https://creativecommons.org/licenses/by-sa/4.0/) . This work is licensed under the same license.

#### Docs
To put your own trigger to count radiation, replace the M5.BtnA.isPressed() with your own value in (line 112)
```c
if (M5.BtnA.isPressed()) {
    if (flag_tick) {
      signalHandler();

      flag_tick = false;
    }
  } else {
    flag_tick = true;
  }
```
The M5.BtnB.isPressed() is the button that resets the accumulation
```c

if (M5.BtnB.isPressed()) {
    if (flag_null) {
      nullRad();

      flag_null = false;
    }
  } else {
    flag_null = true;
  }
  ```
