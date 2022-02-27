# swars

Syndicate Wars port, alternative binary for the classic Bullfrog game.

This is continuation of the project created by Unavowed and Gynvael Coldwind.

## About

Syndicate Wars Port is a port of the 1996 DOS game Syndicate Wars to modern
operating systems. In particular, it runs on GNU, Mac OS and Windows, but it
should also work on any system supported by the SDL library on the i386
architecture.

The port was created by disassembling the original DOS executable and replacing
the DOS-specific parts with platform-independent code, using SDL for input
and video display, Vorbis for music and OpenAL for sound.

Only the modernised executable is provided with this port. This means that game
data are not included. To install the Syndicate Wars Port, you will need to
have the original data from either Syndicate Wars CD, or digital distribution.

## Community

If you wish to discuss the Syndicate Wars Port or if you have any problems,
you may post to the Google group [syndicate-wars-port](http://groups.google.com/group/syndicate-wars-port)
 (you don't need to be a member to post).

## Installation

To install Syndicate Wars Port, you will need either the original Syndicate
Wars PC CD, or digital distribution of the game. There were several physical
versions released. The multi-lingual version was tested and will definitely
work, other version were not tested.

The steps you need to take vary depending on your operating system.

### Installing on GNU or UNIX

If you're installing on GNU or UNIX, you will have to compile from source.
The steps are:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-gnu-or-unix) below to get
   a compiled executable
3. after the build succeeded, do `make install` in the directory where build
   commands were executed, to copy built files into an installation folder
   (`/usr/local/share/swars` by default)
4. insert the Syndicate Wars CD and make sure it is mounted (e.g. in `/media/cdrom`)
5. do `util/install -f SOURCE -t TARGET -l LANG`, where
   * *SOURCE* is the game CD path, like `/media/cdrom`,
   * *TARGET* is the destination path, in this case `/usr/local/share/swars`,
   * *LANG* is the installation language, and can be one of: `eng` `fre` `ita` `spa` `swe`.

If all went well, you can now play the game.

### Installing on Mac OS X

There is support for creating Mac OS X bundles. While you can just install
the game into the UNIX directory structure, using a bundle is a cleaner solution.

#### Making a Mac OS app bundle

If you build an application bundle, you can install the game by just drag-and-drop
into your *Applications* directory.

You can create the bundle by following these instructions:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-mac-os-x) below to get
   a compiled executable
3. after the code is compiled, install the data, as explained in the
   [GNU or UNIX](#installing-on-gnu-or-unix) section, to any directory,
   say `./data` (in case you wonder, the CD drive should be mounted under
   `/Volumes/SYN WARS QA 9.0`)
4. finally, run `util/mkbundle ./data` (replacing `./data` with the directory
   you installed the data to); this will produce an application bundle named
   `Syndicate Wars.app` in the current directory
5. if you want there to be a pretty icon for the bundle, copy `res/swars.icns`
   to `Syndicate Wars.app/Contents/Resources`

### Installing on Windows

There are two ways to install Syndicate Wars Port on Windows - either download
the installer, or download the source code and compile it by hand.
It is recommended to use the installer!

The installer requires the original Syndicate Wars CD to be present in the
CD-ROM drive. It will copy the files from the original Syndicate Wars CD and
optionally encode the game music from the CD to ogg files used by this port.

The Syndicate Wars CD is not required to play the game, it is used only during
the installation process.

If you've decided on the hand-compilation option, proceed with the following steps:

1. download and unpack the source tarball or clone git repo
2. follow [building instructions](#building-on-windows) below to get
   a compiled executable
3. copy all the files and directories, except for the Language directory, from
   the `Game` directory within the *Syndicate Wars CD* to the location of your
   desire, say `SWarsLocation`.
4. copy all the files, except `SOUND.DAT`, from the `Game\Language\LANGUAGE\` from
   the *Syndicate Wars CD* to the `SWarsLocation\Data` directory, where *LANGUAGE*
   stands for the appropriate subdirectory fo the language you want
5. copy the `SOUND.DAT` file (from the directory stated above) to the
   `SWarsLocation\Sound` directory
6. *[optional]* rip the game CD Audio tracks to `track_{1,2,3}.ogg` (vorbis)
7. *[optional]* create a `SWarsLocation\music` directory and copy the
   previously encoded ogg files there
8. copy `swars.exe` to your `SWarsLocation` directory

You can now launch `swars.exe` and have fun!

## Building

Note that you can oly build the port for x86 architecture, and you either need
32-bit OS, or 64-bit OS with 32-bit user space libraries available.

### Building on GNU or UNIX

To build Syndicate Wars Port on GNU or UNIX, you will need the following:

* GNU C compiler
* Python 3
* vorbis-tools (oggenc in particular)
* cdparanoia
* development versions of the following libraries:
  * SDL
  * OpenAL (we recommend OpenAL Soft)
  * libvorbis
  * libogg
  * libpng
  * zlib

Once you've made sure you have the above, proceed with the following steps:

1. go into the directory with `swars` source release (containing `conf`, `doc`, `src` etc.)
2. do `./configure` to make the build scripts find required toolchain and libraries
3. do `make` to compile the executable file

You should now have a working `src/swars` executable file.

#### Build example - Ubuntu 20.04 64-bit

Here are specific commands required to compile the executable on Ubuntu linux.

Install the dependencies - remember that some must be 32-bit (i386):

```
sudo apt install gcc-multilib lib32z1 python3
sudo apt install vorbis-tools
sudo apt install cdparanoia
sudo apt install libsdl1.2-dev:i386
sudo apt install libopenal-dev:i386
sudo apt install libvorbis-dev:i386 libvorbisfile3:i386
sudo apt install libogg-dev:i386
```

In case you want to re-create build scripts from templates (shouldn't be needed):

```
autoreconf
automake --add-missing
```

Now proceed with the build steps; we will do that in a separate folder.

```
mkdir -p release; cd release
CFLAGS="-m32" LDFLAGS="-m32" PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" ../configure --enable-debug=no
make V=1
```
Providing `PKG_CONFIG_PATH` allows us to tell `pkg-config` command where to
search for 32-bit packages (it would use a path to 64-bit ones by default).
The `V=1` variable makes `make` print each command it executes, which makes
diagnosing issues easier.

In case you also want a debug build:

```
mkdir -p debug; cd debug
CFLAGS="-m32" LDFLAGS="-m32" PKG_CONFIG_PATH="/usr/lib/i386-linux-gnu/pkgconfig" ../configure --enable-debug=full
make V=1
```

### Building on Mac OS X

Mac OS X is at its core a UNIX system. To build the Syndicate Wars Port it is
enough to follow the [instructions for GNU or UNIX](#building-on-gnu-or-unix).

The GCC compiler for Mac OS X comes as part of XCode, which you can get from
Apple's web site. You can also make your life a lot easier by using
[Fink](http://www.finkproject.org/) or [MacPorts](http://www.macports.org/)
to install the required packages for you, instead of having to download and
compile them manually.

If you have Mac OS 10.6 ("Snow Leopard") or newer, you will need to make sure
that your libraries contain code for the i386 architecture. In MacPorts this
is achieved by using the `+universal` variant (e.g. `port -vu install libsdl +universal`).

When you configure your compilation toolchain and location of libraries,
you will need to pass `CFLAGS="-arch i386"`, like so:

```
./configure CFLAGS="-arch i386"
```

If you are planning to create [Mac OS app bundle](making-a-mac-os-app-bundle),
you will also need to pass `data-path`, so youf final command will be:


```
./configure --with-data-path="Syndicate Wars.app/Contents/Resources" CFLAGS="-arch i386"
```

Then, do `make` as the [GNU or UNIX](#building-on-gnu-or-unix) instructions tell.

### Building on Windows

To compile the source code you will need the following:

* MinGW C compiler, 32-bit
* Python 3
* development versions of the following libraries:
  * SDL
  * OpenAL (we recommend OpenAL Soft, however Creative Inc. OpenAL can also be used)
  * libvorbis
  * libogg
  * libpng
  * zlib

Then, proceed with the following steps:

1. go into the directory with `swars` source release (containing `conf`, `doc`, `src` etc.)
2. create a directory to store build files, name it `release`
4. edit `src/Makefile.windows` to match your compiler and library locations
5. do `make -C src -f Makefile.windows` to compile the executable file

If you have a proper UNIX-like environment, like MSYS, on your system, you can also
compile the game using the standard `./configure && make`, or cross-compile it for
Windows by passing an appropriate `--host=` argument to `configure`.

#### Build example - MinGW32 on Windows

In this example we will use Minimal GNU for Windows, only. This means no need
for [autotools](https://en.wikipedia.org/wiki/GNU_Autotools) or `configure` script.
But to succeed, the `make` still needs all library dependencies to be accessible,
and the toolchain to be in PATH environment variable.

You may need to edit `src/Makefile.windows` to provide compiler with paths to headers
(by modifying `CFLAGS=`), and linker with paths and names of libraries (`LDFLAGS=`).

We will create a debug build in this example:

```
mkdir debug
mingw32-make -C src -f Makefile.windows V=1 DEBUG=1
```

#### Build example - MSYS2 updated 2022-01 on Windows

Using Minimal System and the MinGW toolchain available within, it is possible
to build the executable using the same way as for UNIX systems, with bash and autotools.

First install the dependencies - mingw32, since we need 32-bit toolchain:

```
pacman -Si mingw-w64-i686-binutils mingw-w64-i686-pkgconf mingw-w64-i686-make mingw-w64-i686-gcc
pacman -Si mingw-w64-i686-libpng
pacman -Si mingw-w64-i686-SDL
pacman -Si mingw-w64-i686-openal
pacman -Si mingw-w64-i686-libvorbis
pacman -Si mingw-w64-i686-libogg
```

In case you want to re-create build scripts from templates (shouldn't be needed):

```
autoreconf
automake --add-missing
```

Now proceed with the build steps; we will do that in a separate folder.
Note how we are modifying PATH environment variable to try mingw32 binaries
before the default mingw64:

```
mkdir -p release; cd release
PATH="/mingw32/bin:$PATH" CFLAGS="-m32" LDFLAGS="-m32" ../configure --enable-debug=no
PATH="/mingw32/bin:$PATH" make V=1
```

## Done

That's all. See more documentation at [swars.vexillium.org](http://swars.vexillium.org/).
