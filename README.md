# echelon-unicode-parser

The unicode specification can be found [here](http://www.unicode.org/versions/latest/)

#### Building the project in the development environtment (eupenv)
The develoment environment needs to be built first. See instructions below.

Run the (PowerShell) command
```cmd
docker run -it -v ${pwd}:/development --name myeupenv eupenv:latest
```
This launches a terminal. You're now in the latest Debian docker image with Gradle and GCC 7.3.0 configured.

Now, inside the environment, the project can be build by running
```cmd
/opt/gradle/gradle-4.9/bin/gradle assemble
```

If you're going to use GDB inside the container you'll need to add some extra flags when you create the container.
These will give the container extra privileges to allow GDB to work
```cmd
docker run -it -v ${pwd}:/development --name myeupenv --cap-add=SYS_PTRACE --security-opt seccomp=unconfined eupenv:latest
```

#### Building the development environment
>Please be aware that the image contains the Oracle JDK (java is needed for Gradle). In order to build the image without user intervention it automatically accepts the Oracle JDK license agreement. So by building the image you are effectively accepting the license agreement. If you wish to review it, you can find it [here](http://www.oracle.com/technetwork/java/javase/terms/license/index.html).

Open a terminal in the ```env``` directory and run
```cmd
docker build . -t eupenv --cpuset-cpus="0-3"
```
This will build the development machine image from the dockerfile. It will be called eupenv and attempt to use 4 CPUs.

The image should now be ready to use.

#### Developing in VSCode

If you want the annoying squigly green lines under your system headers includes to go away. Or just want intelliSense to work! Then the simplest way to do that is to install the VC++ build tools, reboot and just do what VSCode asks.
This does mean you're coding against the Visual C++ headers and building against the GCC headers but let's hope that the ISO standard behind C++ works out. You're also welcome to ignore the docker environment and just build against VC++, but any issues you run into you'll have to figure out for yourself.

#### Things to fix
- The PATH variable in the eupenv is only set in the build session. Which means you have to reference ```/opt/gradle/gradle-4.6/bin/gradle``` when using the image.
- VSCode runs outside the eupenv, so it can't use the header files from in there for intelliSense. Possibly I can map the directory containing the headers from the image.
- Get Gradle to use gcc-7.3.0 rather than the default system gcc in the eupenv. It works perfectly with the default, but throws a fit when you ask it to use the custom build. One possibility is that I uninstall the default gcc before installing the new one - configured without a prefix.
- Build google test into the docker image so it doesn't need to be checked in.
