# e-dlib
 Library and program downloader from terminal.

## Usage:
1. Download `dlib.exe` file from releases tab.
2. Add the downladed file to `PATH` enviroment variable so you can directly use `dlib` at terminal.
3. That's all the instalation of dlib!
## Some commands:
```
dlib cb8
dlib https://some-lib-web.org/install.ds
dlib project/hello-world.ds
dlib //version
```
The first one searched for a script called `cb8` at the `@elitees/dlib-index` GitHub repository and runned it.

The second one searched for a file called `install.ds` on internet at `https://some-lib-web.org/` and runned it.

The third one searched for a file called `hello-world.ds` at the folder `project` and runned it.

You can check dlib version by running `dlib //version`, so be careful to not name a file `//version`.

## Help and documentation
I'm working to do a documentation and help web at `https://e--dlib.web.app`. So currently there's no documentation.

## License
This project is licensed with GNU GPL v3.