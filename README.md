# Tofita

![Tofita Logo](docs/logo.png?raw=true)

All-new kernel to-fit-a [Greentea OS](https://github.com/GreenteaOS) (hence "Tofita")

[![Telegram Channel](https://img.shields.io/badge/Telegram-Greentea%20NEWS-blue.svg)](https://t.me/s/greenteaos_news)
[![Telegram Chatroom](https://img.shields.io/badge/Telegram-Greentea%20OS-blue.svg)](https://telegram.me/greenteaos)
[![Join the chat at Discord!](https://img.shields.io/badge/Discord-Join%20Chat-677bc4.svg)](https://discord.gg/UGZq8GB)
[![GitHub Sponsor](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&color=%23fe8e86)](https://github.com/sponsors/PeyTy)
[![Automatic Builds](https://img.shields.io/badge/Get-Builds-ff69b4.svg?style=flat)](https://ci.appveyor.com/project/PeyTy/tofita/build/artifacts)
[![Made with Hexa](https://img.shields.io/badge/Code-Hexa-FF7F50.svg?style=flat)](https://github.com/hexalang/hexa)

Greentea OS is aimed to provide complete operating system compatible with existing apps

![Screenshot](https://raw.githubusercontent.com/GreenteaOS/Greentea/kawaii/Images/screenshot.jpg)

## GreenteaOS (Tofita) system requirements

- 64-bit UEFI
- Very low on resources, just 2 GB RAM minimum required
- Motherboard with SATA (AHCI) or NVMe, PCIe and USB 2.0+
- Multicore modern 64-bit CPU (most AMD and Intel CPUs from [Q3 2011 and newer](https://github.com/GreenteaOS/Greentea/blob/kawaii/Developer-Guide/CPUID.md#notes-on-cpu-compatibility))
  - Try [Greentea OS Compatibility Assistant on your PC](https://t.me/s/greenteaos_official/7)

[![Give a star](docs/star.png?raw=true)](https://github.com/GreenteaOS/Tofita/stargazers)

Give some attention for Greentea OS be noticed

[![Become a sponsor](docs/sponsors.png?raw=true)](https://github.com/sponsors/PeyTy)
[![Become a patron](docs/patreon.png?raw=true)](https://www.patreon.com/PeyTy)

Become a sponsor to help hire developers and test more hardware

## Tools

- [`listen.js`](tools/listen.js) - listen to VirtualBox COM port in TCP socket mode
- [`pipe.js`](tools/pipe.js) - listen to VirtualBox\Hyper-V COM port in pipe mode
- [`vbox.js`](tools/vbox.js) - starts VirtualBox Tofita VM and exits
- [`webui.bat`](tools/webui/webui.bat) - web-based UI for COM port
- [`gos-devlist.bat`](tools/gos-devlist.bat) - show useful device names and PCI IDs to gather user hardware stats
- [`OVMF.fd`](https://t.me/s/greenteaos_official/6) - speed of light UEFI bootloader for QEMU
- [`Greentea OS Compat`](https://t.me/s/greenteaos_official/7) - Greentea OS compatibility assistant

## Build

Get automatic build or make yours with Teapot (if you want to edit the code)

### Automatic builds

If you don't want to build Tofita, you may [download latest automatic GitHub build](https://ci.appveyor.com/project/PeyTy/tofita/build/artifacts).
Note that it also builds third-party pull requests.


### Build using Teapot

Download or clone [Teapot repo](https://github.com/GreenteaOS/Teapot#download-latest-zip) and others. Place alongside of Tofita repo:

```
.\Teapot
.\Tofita
.\Greentea
.\Tongue
```

You should have extracted `Teapot-kawaii` and have folders like this `.\Teapot\LLVM-*.*.*`.

Teapot will take less space if cloned with limited depth of commit history:

```sh
git clone --recursive --depth 1 https://github.com/GreenteaOS/Tofita.git
git clone --recursive --depth 1 https://github.com/GreenteaOS/Teapot.git
git clone --recursive --depth 1 https://github.com/GreenteaOS/Greentea.git
git clone --recursive --depth 1 https://github.com/GreenteaOS/Tongue.git
```

#### Configure

Add your apps, folders and other files into `assets\desktop` of the `Greentea` repo to see them in the built image.

Create `R:` ram disk about 1 GB (you may download a free [AMD Radeon™ RAMDisk](http://radeonramdisk.com/files/Radeon_RAMDisk_4_4_0_RC36.msi)) for faster builds and configure `config.json > tempFolder`.
Otherwise `C:` is used to store builds by default.

#### Install Hexa

You also need to install [Hexa compiler](https://github.com/hexalang/hexa#unstable).

Currently you will build Hexa manually too. Download [this](https://github.com/hexalang/hexa/archive/kawaii.zip) and extract contents of `hexa-kawaii` into `C:\Hexa`, so you have `C:\Hexa\hexa.cmd`.

Install [node.js LTS](https://nodejs.org/en/download/).

```sh
cd C:\hexa
node bootstrap.js hexa.json
node hexa-node.js hexa.json
node hexa-node.js hexa.json
```

Add `C:\Hexa` into your PATH.

#### Compile and generate disk image

```sh
cmd /c build
```

Or just open `build.bat` file with Explorer.

## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

[More information about legal issues](https://github.com/GreenteaOS/Greentea/blob/kawaii/README.md#license)
