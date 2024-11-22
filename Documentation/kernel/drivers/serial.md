# Serial Driver for CeanOS 

## Overview

This code provides basic functionality to interact with the serial ports (specifically COM1) in the context of CeanOS. It includes initialization, device creation, and basic read/write operations for the serial device. The code interacts with the VFS(Not done yet) to mount and interact with the serial port as a device node under `/dev/COM1`.

## Functions

### 1. `int init_serial_dev()`

**Purpose**: Initializes the serial device at COM1 and attempts to mount it under `/dev/COM1`.

**Parameters**: None

**Returns**: 
- `ERR_CANT_OPEN` if the `/dev` directory cannot be opened.
- `SUCCESS` if the initialization and mounting are successful.
- `NULL` if the COM1 device creation fails.

**Description**:
- Opens the `/dev` directory using VFS.
- Attempts to create a serial device for COM1 using `serial_create_device`.
- If successful, the device is mounted under `/dev/COM1`. If mounting fails, the device is freed.
- Closes the `/dev` directory after operations are complete.

---

### 2. `int serial_open(vfs_node *node)`

**Purpose**: Opens the serial device node for reading or writing.

**Parameters**: 
- `node`: A pointer to the VFS node representing the serial device.

**Returns**:
- `ERR_CANT_OPEN` if the node cannot be opened.
- `SUCCESS` if the device is successfully opened.

**Description**:
- Checks if the node has an associated inode. If not, it returns an error.
- If the inode is valid, the serial port is opened successfully.

---

### 3. `int serial_close(vfs_node *node)`

**Purpose**: Closes the serial device node.

**Parameters**: 
- `node`: A pointer to the VFS node representing the serial device.

**Returns**: 
- `SUCCESS` (this function currently doesn't handle closing logic but could be expanded for cleanup in the future).

**Description**:
- This function is a placeholder for closing the serial device. It currently always returns `SUCCESS`.

---

### 4. `ssize_t serial_write(vfs_node *node, off_t offset, size_t count, char *buffer)`

**Purpose**: Writes data to the serial device.

**Parameters**: 
- `node`: A pointer to the VFS node representing the serial device.
- `offset`: The offset within the device (currently ignored).
- `count`: The number of bytes to write.
- `buffer`: The data buffer to write.

**Returns**: 
- The number of bytes written if successful.
- `ERR_BAD_INPUT` if the input is invalid.
- `ERR_CANT_WRITE` if the device cannot be written to.

**Description**:
- Writes the provided buffer to the serial port, one byte at a time, using `serial_write_char`.
- The `offset` parameter is ignored, as serial communication doesn't need to seek or handle offsets.

---

### 5. `vfs_node *serial_create_device(uint16_t port)`

**Purpose**: Creates and initializes a serial device for a specific port.

**Parameters**:
- `port`: The port number to initialize (e.g., `COM1`).

**Returns**: 
- A pointer to a `vfs_node` representing the serial device if successful.
- `NULL` if the serial port initialization fails.

**Description**:
- Initializes the serial port by writing to several I/O ports to configure the UART (Universal Asynchronous Receiver-Transmitter) settings (baud rate, line control, interrupt enable, etc.).
- Verifies the port initialization by checking the device status.
- Allocates and returns a `vfs_node` that represents the device, with the appropriate read/write functions (`serial_open`, `serial_close`, `serial_write`).

---

### 6. `void serial_write_char(char ch, uint16_t port)`

**Purpose**: Writes a single character to the serial port.

**Parameters**: 
- `ch`: The character to write.
- `port`: The serial port (e.g., `COM1`).

**Returns**: 
- None

**Description**:
- Waits for the serial port to be ready to transmit data by checking the line status register.
- Once ready, it writes the character to the serial port.

---

## Error Codes

- `ERR_CANT_OPEN`: Unable to open the specified device or file.
- `ERR_CANT_WRITE`: Unable to write to the device.
- `ERR_BAD_INPUT`: Invalid input was provided.
- `SUCCESS`: The operation was successful.

## Notes

- This driver currently supports only COM1, but can be extended to support other COM ports.
- The `serial_write` function ignores the `offset` parameter, as the serial port is a linear stream.
- Error handling is minimal, and more robust checks and feedback could be added as needed.
