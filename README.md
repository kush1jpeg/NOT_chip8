```bash
M"""""""`YM MMP"""""YMM M""""""""M    MM'""""'YMM M""MMMMM""MM M""M MM"""""""`YM .d888b. 
M  mmmm.  M M' .mmm. `M Mmmm  mmmM    M' .mmm. `M M  MMMMM  MM M  M MM  mmmmm  M Y8' `8P 
M  MMMMM  M M  MMMMM  M MMMM  MMMM    M  MMMMMooM M         `M M  M M'        .M d8bad8b 
M  MMMMM  M M  MMMMM  M MMMM  MMMM    M  MMMMMMMM M  MMMMM  MM M  M MM  MMMMMMMM 88` `88 
M  MMMMM  M M. `MMM' .M MMMM  MMMM    M. `MMM' .M M  MMMMM  MM M  M MM  MMMMMMMM 8b. .88 
M  MMMMM  M MMb     dMM MMMM  MMMM    MM.     .dM M  MMMMM  MM M  M MM  MMMMMMMM Y88888P 
MMMMMMMMMMM MMMMMMMMMMM MMMMMMMMMM oo MMMMMMMMMMM MMMMMMMMMMMM MMMM MMMMMMMMMMMM         
```

---

## 📼 What is CHIP-8 ?  

Forget pixels in HD. Forget shaders.  

CHIP-8 is a virtual 8-bit machine developed in the late 1970s to run simple games on tiny computers like the COSMAC VIP and HP-48. It’s not real hardware—it’s an interpreter that emulates a machine with:
* 64×32 monochrome display
* 16-key hexadecimal keypad
* 35 opcodes for graphics, math, timers, and input
* Sound & delay timers ticking at 60Hz

Later on, clever hackers discovered that CHIP-8’s tiny virtual machine could fit inside pocket calculators and minimalistic microcomputers, including some models from HP. Enthusiasts ported interpreters to these devices, running classic CHIP-8 games on calculators

It’s a machine with a legacy, acting as the  first project of many computer/emulator enthusiasts,.

---

## ⚙️ Features  
```bash
┌───────────────────────────────────────────────┐
│          ░░░ CHIP-8 CORE FUNCTIONS ░░░        │
├───────────────────────────────────────────────┤
  ⚡ OPCODES          : 35 instructions of pure 
│                      8-bit power              │
  ⏱ TIMERS           : 60Hz heartbeat, synced   
│                      like a metronome         │
  ⌨ HEX KEYPAD       : Map your keys to the     
│                      ancient 0x0 → 0xF        │
  🎨 GRAPHICS + SOUND : SDL2 renders, beeps     
│                      at your command          │
  💾 ROM LOADER       : Load classic games and  
│                      relive the 70s           │
└───────────────────────────────────────────────┘

```

---
## DEMO 
![Pong in action](./assets/pong.gif)

---
## 🚀 Running the Emulator  

### Build

Dependencies:

- `gcc` or `clang`
- `SDL2`  

```bash
git clone https://github.com/kush1jpeg/NOT_chip8.git
cd NOT_chip8
make run


