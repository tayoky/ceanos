# CeanOS

![GitHub top language](https://img.shields.io/github/languages/top/asdasda3456/CeanOS)
![Contributors](https://img.shields.io/github/contributors/asdasda3456/CeanOS)
![Commits](https://img.shields.io/github/commit-activity/m/asdasda3456/CeanOS)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/asdasda3456/CeanOS)
![GitHub Release](https://img.shields.io/github/v/release/asdasda3456/CeanOS?include_prereleases)
![GitHub repo size](https://img.shields.io/github/repo-size/asdasda3456/CeanOS)

![image](https://github.com/user-attachments/assets/d020d976-7440-4d49-8e8b-25f0f7a2cdd4)

## Kernel Status
The CeanOS kernel is a monolithic x86 one. I am also planning to port it to x86_64 and arm after im done with x86 .

## Other Information
The file `vga_types.h` contains macros taken from the Linux source code, which can be found [here](https://github.com/torvalds/linux/blob/42f7652d3eb527d03665b09edac47f85fb600924/include/video/vga.h).

## Goals
- **Kernel**
  - [x] Basic kernel operations
  - [ ] Usable terminal
- **Architecture**
  - [x] x86 (WIP)
  - [ ] x86_64
  - [ ] ARM
- **Filesystems**
  - [ ] FAT32
  - [ ] ext2
  - [ ] FAT32 read
  - [ ] ext2 read
  - [ ] FAT32 write
  - [ ] ext2 write
  - [ ] VFS
- **Device Drivers**
  - [x] Generic: Keyboard, VGA, PIT
  - [ ] Storage: SSD, USB, HDD, AHCI
  - [ ] ACPI and others
- **Networking**
  - [ ] Drivers: idk
  - [ ] Additional features: idk (still researching)
- **Graphics**
  - [x] VGA text mode
  - [ ] Framebuffer, GUI, idk?

## Compiling
Everything about it can be found in `docs/build.md` or [here](docs/build.md)

## License
This project is licensed under GPL v3 (GNU General Public License v3.0). For more information, see the [LICENSE](LICENSE).
