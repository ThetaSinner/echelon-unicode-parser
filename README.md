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

#### Developing in Visual Studio

This is currently the only way the project can be debugged graphically. If you've got a simple problem then GDB is available in the linux docker container.

Installing the Community edition of Visual Studio will result in Gradle being unable to locate VS tools. You need to include the C++ desktop development package from the VS installer. Gradle will actually use a tool in the installer to locate the tools it needs.

If you have problems building on VC++ then enable debug mode for Gradle using `gradle build --debug`. Looking through the logs should help to identify what Gradle is actually doing and lead you to a solution, or at least help from the internet.

None of the tests or benchmarks can be used on Windows at the moment. The Google tools they depend on need to be built with CMake and I've had no need to put myself through that yet, hence - no example or instructions for doing this are provided. This means that the test and benchmark binaries are not built when Gradle is running on Windows.

#### Benchmarking

When you build the project a benchmark executable is built. It uses Google's C++ [benchmark](https://github.com/google/benchmark) tool to do the timing.

Inside the bench directory you'll find 3 shell scripts.

| Name             | Description                                                                                  |
|------------------|----------------------------------------------------------------------------------------------|
| run-bench.sh     | Runs the benchmark executable and captures results into a results file ending in `_dev.json` |
| compare-bench.sh | Compares the `_dev.json` to the accepted results and prints the results for comparison       |
| save-bench.sh    | To update the accepted results this copies the `_dev.json` results to the accepted results   |

The dev results are ignored, and the accepted results are version controlled.

#### Things to fix
- The PATH variable in the eupenv is only set in the build session. Which means you have to reference ```/opt/gradle/gradle-4.6/bin/gradle``` when using the image.
- VSCode runs outside the eupenv, so it can't use the header files from in there for intelliSense. Possibly I can map the directory containing the headers from the image.
- Get Gradle to use gcc-7.3.0 rather than the default system gcc in the eupenv. It works perfectly with the default, but throws a fit when you ask it to use the custom build. One possibility is that I uninstall the default gcc before installing the new one - configured without a prefix.
- Build google test into the docker image so it doesn't need to be checked in.
