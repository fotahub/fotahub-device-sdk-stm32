# FotaHub&reg; Device SDK for STM32&reg;
The FotaHub&reg; Device SDK for STM32&reg; enables developers to create firmware for STM32&reg;-based IoT devices that can be updated through [FotaHub](http://fotahub.com). It provides a FotaHub client C library along with a set of examples illustrating how the latter can be used and integrated in real applications.

# Supported targets
The FotaHub&reg; Device SDK for STM32&reg; can be used on STM32
Nucleo&trade; development boards or any other STM32&reg;-based hardware product that feature an on-board Ethernet network interface or a wireless Wi-Fi adapter. 

# Installation
To use the FotaHub&reg; Device SDK for STM32&reg;, the following tools must be available on your laptop or computer:
* The [STM32CubeIDE 1.9.0](https://www.st.com/en/development-tools/stm32cubeide.html) or later (based on Eclipse)
* The [STM32CubeProg 2.10.0](https://www.st.com/en/development-tools/stm32cubeprog.html) or later
* The [FotaHub Eclipse Plugin 0.1.0](https://marketplace.eclipse.org/content/fotahub-eclipse-plugin) or later  
  [![Drag to your running STM32CubeIDE workspace](https://marketplace.eclipse.org/sites/all/themes/solstice/public/images/marketplace/btn-install.svg)](http://marketplace.eclipse.org/marketplace-client-intro?mpc_install=5347433 "Drag to your running STM32CubeIDE workspace")
* The [TM Terminal 10.5.0](https://marketplace.eclipse.org/content/tm-terminal) Eclipse plugin or later  
[![Drag to your running STM32CubeIDE workspace](https://marketplace.eclipse.org/sites/all/themes/solstice/public/images/marketplace/btn-install.svg)](http://marketplace.eclipse.org/marketplace-client-intro?mpc_install=1687099 "Drag to your running STM32CubeIDE workspace")  
Be sure to select (at least) both the required **TM Terminal** base feature as well as the optional **TM Terminal Serial Connector Extensions** feature when prompted to do so during the installation procedure

# Getting started
The best way to get started with the FotaHub&reg; Device SDK for STM32&reg; is to try out and study the included examples:
* [Simple](docs/getting-started/simple.md) - "Hello World" of firmware over-the-air updating with FotaHub&reg;
* [Rollbackable](docs/getting-started/rollbackable.md) - Downloads and launches a new firmware version provided at FotaHub&reg;, rolls it back to the previous version if something goes wrong
* [Thing Shadow](docs/getting-started/thingshadow.md) - A real-world scenario where FotaHub&reg; is used in combination with the AWS IoT Thing Shadow service to trigger firmware updates over-the-air updates through the cloud

# API Reference
View the [C API reference](docs/reference/c-api.md) for detailed information about the FotaHub&reg; client library.

# License
Unless specifically indicated otherwise in a file included in this project, FotaHub&reg; Device SDK for STM32&reg; files are provided under the *Apache-2.0* license. See the [LICENSE](LICENSE) file for the full text of this license.

The terms of the license attributed to this project do not alter any rights or obligations you may have under the licenses of any third-party software which is redistributed through the present project. Refer to the [NOTICE](NOTICE.md) file for detailed information regarding these aspects.
