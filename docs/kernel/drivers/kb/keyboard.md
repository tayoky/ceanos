# Keyboard Driver for CeanOS

This document provides an overview of the keyboard driver used in CeanOS, detailing the available functions.

## Overview

The CeanOS keyboard driver handles keyboard events by reading scan codes, processing them, and updating the text output accordingly. It uses an interrupt-based system to respond to key events.

## Limits

When the enter key (`'\n'`) is pressed, the following sequence occurs in the `keyboardHandler` function:

```c
if (charToAdd == '\n') {
    print("\n");
    splitter(text); 
    clear();         
    if (safe_mode) {
        ...
    }
}
```

### `splitter(text)` function

The `splitter(text)` function processes terminal input. It’s triggered when the ENTER key is pressed.

Note that the terminal system is directly integrated into the kernel for now. This function serves as a temporary placeholder for testing until a fully functional shell is implemented.

The `splitter()` function will eventually be replaced by a more sophisticated command parsing and handling system located in the userland, separate from the kernel.

For now, this terminal-like functionality is strictly for testing and debugging purposes.

## Key Variables

- **capsOn**: Boolean flag indicating whether the Caps Lock key is being pressed.
- **capsLock**: Boolean flag indicating whether Caps Lock is on or off.
- **text**: A buffer storing the current input text.
- **KEY_COUNT**: Constant representing the number of keys supported (128 keys).

## Special Key Constants

These constants represent special keys and events:

- **UNKNOWN**: Represents an unrecognized key.
- **ESC, CTRL, LSHFT, RSHFT, ALT**: Special keys (Escape, Control, Left Shift, Right Shift, Alt).
- **F1-F12**: Function keys (F1 to F12).
- **SCRLCK, HOME, UP, DOWN, LEFT, RIGHT**: Navigation keys (Scroll Lock, Home, Arrow keys, etc.).
- **CAPS**: Caps Lock key.
- **NUMLCK**: Numeric Lock key.
- **INS, DEL**: Insert and Delete keys.

## Key Mappings

- **lowercase**: An array containing lowercase character mappings for the keyboard, indexed by scan codes.
- **uppercase**: An array containing uppercase character mappings for the keyboard, indexed by scan codes.

## Functions

### `void append(char *part)`

Appends a character to the `text` buffer. If the buffer is not full, the character is added and the string is null-terminated.

### `void rm()`

Removes the last character from the `text` buffer, performing a backspace operation. The buffer is updated by decrementing the last character and null-terminating the string.

### `static void clear()`

Clears the `text` buffer by setting the first character to the null terminator (`'\0'`).

### `void parser(uint8_t code)`

This function reads a scan code, compares it with the `text` buffer, and prints characters up to the matched code. It prints the buffer up to that point if the character at the current position matches the expected one.

### `static void updateTextBuffer(uint8_t code, uint8_t press)`

Updates the `text` buffer with the character corresponding to the given `scanCode`. It checks the Caps Lock and Shift keys to decide whether the character should be uppercase or lowercase. It also processes key presses and handles actions when the Enter key is pressed (e.g., splitting the text into a command).

- **Parameters**:
  - `code`: Scan code of the key pressed.
  - `press`: Key press state (0 for key press, non-zero for key release).

### `static void keyboardHandler(struct InterruptRegisters *regs)`

Handles keyboard interrupt events, processing scan codes and determining key actions. It checks key press events, updates the Caps Lock state, handles backspace, and invokes `updateTextBuffer()` for normal character input.

- **Parameters**:
  - `regs`: Interrupt registers, containing CPU state during the interrupt.

### `void keyboard_init()`

Initializes the keyboard driver by setting the `capsOn` and `capsLock` flags to false, installing the keyboard interrupt handler, clearing the `text` buffer, and printing a message indicating the keyboard is enabled.

## Key Features

- **Shift and Caps Lock Support**: The driver handles the Caps Lock and Shift keys to toggle between lowercase and uppercase characters.
- **Backspace Handling**: Supports backspace functionality to remove the last character typed.
- **Text Buffer Management**: Dynamically manages the `text` buffer as keys are pressed and backspaces are performed.

## Special Key Handling

- **Caps Lock**: Toggles the `capsLock` variable. When active, it forces all characters to be typed in uppercase.
- **Shift Keys**: Shift keys toggle the `capsOn` flag, which temporarily overrides the Caps Lock state for the current key press.

## Other

The keyboard driver operates in an interrupt-driven manner. The `keyboardHandler()` function is registered with the IDT and is called whenever the keyboard sends an interrupt. It processes the scan codes in real time.

