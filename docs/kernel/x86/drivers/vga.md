# VGA Driver for CeanOS

This document provides an overview of the VGA driver in CeanOS, detailing its functionality and available functions.

The CeanOS VGA driver uses an 8-bit color system.

## Functions

### `inline void Reset()`

Clears the screen and resets it to its initial state.

### `inline void newLine()`

Inserts a new line in the terminal, moving the cursor to the beginning of the next line.

### `inline void scrollUp()`

Scrolls the screen content up by one line.

### `void print(const char* s)`

Prints the string `s` to the screen. The function supports the following special characters:

- `'\n'` (new line)
- `'\r'` (carriage return)
- `'\b'` (backspace)
- `'\t'` (tab)

These characters behave like in standard C, allowing basic text formatting.

### `void set_screen_color(uint8_t color)`

Sets the background and foreground colors of the screen using an 8-bit hexadecimal value (e.g., `0x0F` for white text on a black background). It directly interacts with memory at address `0xC00B8000` to update the colors.

### `void vga_disable_cursor()`

Disables the VGA cursor, making it invisible.
