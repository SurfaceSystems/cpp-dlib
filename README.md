# dlib
Library and program downloader from terminal.
Works on Windows and Linux :)

## Installation
### 1. Windows
#### 1. From releases
1. Download `dlib.exe` file from releases tab.
2. Add the downladed file to `PATH` enviroment variable so you can directly use `dlib` at terminal.
#### 2. From actions
1. Go to actions and search for a Release action.
2. Download the windows artifact of that action.
3. Add the downloaded file to `PATH` so you can use dlib from the terminal
#### 3. Installer
Not implemented by now
#### 4. Compiling
Not implemented by now

### 2. Linux
#### 1. Compiling
Not implemented by now

## Some commands:
```
dlib cb8
dlib https://some-lib-web.org/install.dls
dlib project/hello-world.dls
dlib --version
```
The first one searched for a script called `cb8` at the repositories you have configured.

The second one searched for a file called `install.dls` on internet at `https://some-lib-web.org/` and runned it.

The third one searched for a file called `hello-world.dls` at the folder `project` and runned it.

You can check dlib version by running `dlib --version`, so be careful to not name a file `--version`.

## Help and documentation
Here you can find documentation:
[https://surfacesystems.github.io/cpp-dlib/docs/](https://surfacesystems.github.io/cpp-dlib/docs/)

## License
This project is licensed with GNU GPL v3.
