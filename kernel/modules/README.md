# Custom Kernel Module for Networking Extension

This project demonstrates how to create a custom Linux kernel module designed to extend the kernel's networking capabilities by adding new features.

## Usage Instructions

The directory contains a `Makefile` with predefined targets to manage the build and installation process:

### Targets

- **build**: Compile the kernel module. This will generate a `.ko` file that can be loaded into the kernel.
  
- **install**: Copy the compiled `.ko` module into the shared folder accessible within your VM. This shared folder is linked via a symbolic link named `shared` in the current directory, allowing easy access from the Guest OS running in the VM.
  
- **clean**: Remove all build artifacts, including the `.ko` file, to clean up the directory for a fresh build.

## How to Use

1. **Build the module**

   ```bash
   make build
   ```

2. **Install the module**

   ```bash
   make install
   ```

3. **Clean build artifacts**

   ```bash
   make clean
   ```

Ensure that your environment has the necessary kernel headers and build tools installed to successfully compile the module.

---

*Note:* The actual kernel module source code should be in the `linux` directory, and the `Makefile` is configured to compile it accordingly.

## M9 – ICMP Echo Monitor

### Features
- Counts IPv4 Echo Request packets
- Counts IPv4 Echo Reply packets
- Counts IPv6 Echo Request packets
- Counts IPv6 Echo Reply packets

### Testing
- IPv4 functionality was tested successfully using `ping`.
- IPv6 support was implemented in the module.
- IPv6 runtime testing could not be completed in the VMware environment because only link-local IPv6 addresses were available, and `ping -6` returned "Network is unreachable".



