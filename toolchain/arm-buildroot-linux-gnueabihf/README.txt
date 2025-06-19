armv7-eabihf--glibc--stable-2018.11-1


Toolchains are hosted here: http://toolchains.bootlin.com/

All the licenses can be found here: http://toolchains.bootlin.com/downloads/releases/licenses/
All the sources can be found here: http://toolchains.bootlin.com/downloads/releases/sources/

PACKAGE      VERSION                   LICENSE
buildroot    2018.08.1-00003-g576b333  GPL-2.0+
gcc-final    7.3.0                     unknown
bison        3.0.4                     GPL-3.0+
lzip         1.20                      GPL-2.0+
tar          1.29                      GPL-3.0+
m4           1.4.18                    GPL-3.0+
gawk         4.2.1                     GPL-3.0+
gcc-initial  7.3.0                     unknown
binutils     2.29.1                    GPL-3.0+, libiberty LGPL-2.1+
gmp          6.1.2                     LGPL-3.0+ or GPL-2.0+
mpc          1.0.3                     LGPL-3.0+
mpfr         3.1.6                     LGPL-3.0+
gdb          7.12.1                    GPL-2.0+, LGPL-2.0+, GPL-3.0+, LGPL-3.0+
expat        2.2.5                     MIT
pkgconf      0.9.12                    pkgconf license
ncurses      6.1                       MIT with advertising clause
patchelf     0.9                       GPL-3.0+
glibc          glibc-2.27-57-g6c99e37f6fb640a50a3113b2dbee5d5389843c1e  GPL-2.0+ (programs), LGPL-2.1+, BSD-3-Clause, MIT (library)
linux-headers  4.1.52                                                   GPL-2.0
dash           0.5.10.2                                                 BSD-3-Clause, GPL-2.0+ (mksignames.c)
gdb            7.12.1                                                   GPL-2.0+, LGPL-2.0+, GPL-3.0+, LGPL-3.0+

For those who would like to reproduce the toolchain, you can just follow these steps:

    git clone https://github.com/bootlin/buildroot-toolchains.git buildroot
    cd buildroot
    git checkout toolchains.bootlin.com-stable-2018.11-1

    curl http://toolchains.bootlin.com/downloads/releases/toolchains/armv7-eabihf/build_fragments/armv7-eabihf--glibc--stable-2018.11-1.defconfig > .config
    make olddefconfig
    make

This toolchain has been built, and the test system built with it has
successfully booted.
This doesn't mean that this toolchain will work in every cases, but it is at
least capable of building a Linux kernel with a basic rootfs that boots.
FLAG: TEST-OK
