CPUchain Core
=============

Setup
---------------------
CPUchain Core is the original CPUchain client and it builds the backbone of the network. It downloads and, by default, stores the entire history of CPUchain transactions (which is currently more than 100 GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download CPUchain Core, visit [cpuchain.org](https://cpuchain.org/download).

Running
---------------------
The following are some helpful notes on how to run CPUchain on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/cpuchain-qt` (GUI) or
- `bin/cpuchaind` (headless)

### Windows

Unpack the files into a directory, and then run cpuchain-qt.exe.

### OS X

Drag CPUchain-Core to your applications folder, and then run CPUchain-Core.

### Need Help?

* See the documentation at the [CPUchain Homepage](https://cpuchain.org)
for help and more information.

Building
---------------------
The following are developer notes on how to build CPUchain on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [OS X Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The CPUchain repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/cpuchain/doxygen/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [Travis CI](travis-ci.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
