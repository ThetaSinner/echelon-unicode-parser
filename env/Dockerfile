FROM amd64/debian:latest

LABEL Author=ThetaSinner

RUN apt-get update && apt-get install -y --no-install-recommends \
    apt-utils \
    build-essential \
    git \
    wget \
    ca-certificates \
    unzip \
    software-properties-common \
    gnupg \
    dirmngr \
    gcc-multilib \
    # For GDB docs
    texinfo \
    && apt-get clean

RUN echo oracle-java10-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EA8CACC073C3DB2A && \
    add-apt-repository ppa:linuxuprising/java && \
    apt-get update && \
    apt-get install oracle-java10-installer -y && \
    java --version

ENV GRADLE_VERSION 4.9
RUN mkdir -p /opt/gradle && cd /opt/gradle && \
    wget https://downloads.gradle.org/distributions/gradle-$GRADLE_VERSION-bin.zip && \
    unzip gradle-$GRADLE_VERSION-bin.zip && \
    export PATH=$PATH:/opt/gradle/gradle-$GRADLE_VERSION/bin && \
    cd / && \
    gradle --version

ENV GCC_VERSION 8.2.0
RUN mkdir -p /opt/gcc && cd /opt/gcc && \
    wget ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz && \
    tar xf gcc-$GCC_VERSION.tar.xz && \
    cd gcc-$GCC_VERSION && \
    ./contrib/download_prerequisites && \
    cd .. && \
    mkdir build && cd build && \
    $PWD/../gcc-$GCC_VERSION/configure --program-suffix=-$GCC_VERSION --disable-bootstrap --enable-languages=c,c++ && \
    make -j 4 && \
    make install && \
    gcc-$GCC_VERSION --version

ENV GDB_VERSION 8.1
RUN mkdir -p /opt/gdb && cd /opt/gdb && \
    wget http://ftp.gnu.org/gnu/gdb/gdb-$GDB_VERSION.tar.xz && \
    tar xf gdb-$GDB_VERSION.tar.xz && \
    mkdir build && cd build && \
    $PWD/../gdb-$GDB_VERSION/configure --program-suffix=-$GDB_VERSION && \
    make -j 4 && \
    make install && \
    gdb-$GDB_VERSION --version

WORKDIR /development
