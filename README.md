# echelon-unicode-parser

#### Building the project in the development environtment (eupenv)
The develoment environment needs to be built first. See instructions below.

Run the (PowerShell) command
```cmd
docker run -it -v "($pwd):/development" eupenv:latest
```
This launches a terminal. You're now in the latest Debian docker image with Gradle and GCC 7.3.0 configured.

Now, inside the environment, the project can be build by running
```cmd
/opt/gradle/gradle-4.6/bin/gradle assemble
```

#### Building the development environment
>Please be aware that the image contains the Oracle JDK (java is needed for Gradle). In order to build the image without user intervention it automatically accepts the Oracle JDK license agreement. So by building the image you are effectively accepting the license agreement. If you wish to review it, you can find it [here](http://www.oracle.com/technetwork/java/javase/terms/license/index.html).


Open a terminal in the ```env``` directory and run
```cmd
docker build . -t eupenv --cpuset-cpus="0-3"
```
This will build the development machine image from the dockerfile. It will be called eupenv and attempt to use 4 CPUs.

The image should now be ready to use.

#### Things to fix
- The PATH variable in the eupenv is only set in the build session. Which means you have to reference ```/opt/gradle/gradle-4.6/bin/gradle``` when using the image.
- VSCode runs outside the eupenv, so it can't use the header files from in there for intelliSense. Possibly I can map the directory containing the headers from the image.
- Get Gradle to use gcc-7.3.0 rather than the default system gcc in the eupenv. It works perfectly with the default, but throws a fit when you ask it to use the custom build. One possibility is that I uninstall the default gcc before installing the new one - configured without a prefix.
