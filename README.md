`measure-evdev` uses Linux’s evdev API to receive key presses and react to a
Caps Lock keypress as quickly as it can by turning the Caps Lock LED on.

## Installation

```
sudo apt install build-essential
git clone https://github.com/kinx-project/measure-evdev
cd measure-evdev
make
```

## Usage

```
sudo ./measure-evdev /dev/input/eventN
```
