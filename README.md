# beroot

beroot is an extremely simple and barebones setuid-root helper. It may have lots of undiscovered vulnerabilities, so only use this if you know what you're doing.

## What works

* ability to interact with executables as uid 0
* asks for user password for authentication

## TODO:

* only allow users listed in config file and be in a specific group to use beroot
* allow morphing into some other user to run commands
* ....and lots more.

## Installation

```shell
make && sudo make install
```

will automatically build and install beroot into `/usr/bin`.
If you wish to choose a separate prefix, pass DESTDIR into `make install` with absolute path to your directory.