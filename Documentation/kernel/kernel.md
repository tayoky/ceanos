### **Kernel Overview**

The **CeanOS** kernel is designed as a **monolithic kernel**, meaning that all core system functionalities such as process management, memory management, file system management, device drivers, and system calls are tightly integrated into a single unit.

The `docs/kernel/` folder contains documentation about CeanOS.

---

#### **Key Features of CeanOS Kernel**:
1. **Unified Architecture**: All essential services (interrupt handling, device management, etc.) are part of the kernel, making it faster but also less modular than microkernels.
2. **Memory Management**: CeanOS’s kernel is responsible for managing physical and virtual memory.
3. **Device Drivers**: The kernel directly interacts with hardware devices (e.g., VGA, keyboard, and disk).

---

#### **Why a Monolithic Kernel?**
- **Performance**: Monolithic kernels are typically faster because they allow direct communication between subsystems, without needing context switches between separate modules.
- **Simplicity**: A single integrated kernel is easier to manage in terms of interactions between subsystems.
- **Flexibility**: While not as modular as microkernels, a monolithic kernel still allows you to implement many features (like loadable kernel modules) for expanding functionality over time.
