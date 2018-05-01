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
    && apt-get clean

RUN echo oracle-java10-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys EA8CACC073C3DB2A && \
    add-apt-repository ppa:linuxuprising/java && \
    apt-get update && \
    apt-get install oracle-java10-installer -y && \
    java --version

RUN mkdir -p /opt/gradle && cd /opt/gradle && \
    wget https://downloads.gradle.org/distributions/gradle-4.7-bin.zip && \
    unzip gradle-4.7-bin.zip && \
    export PATH=$PATH:/opt/gradle/gradle-4.7/bin && \
    cd / && \
    gradle --version

RUN mkdir -p /opt/gcc && cd /opt/gcc && \
    wget ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases/gcc-7.3.0/gcc-7.3.0.tar.gz && \
    tar xzf gcc-7.3.0.tar.gz && \
    cd gcc-7.3.0 && \
    ./contrib/download_prerequisites && \
    cd .. && \
    mkdir objdir && \
    cd objdir && \
    $PWD/../gcc-7.3.0/configure --program-suffix=-7.3.0 --disable-bootstrap --enable-languages=c,c++ && \
    make -j 4 && \
    make install && \
    gcc-7.3.0 --version

RUN mkdir -p /opt/antlr4 && cd /opt/antlr4 && \
    wget http://www.antlr.org/download/antlr-4.7.1-complete.jar

RUN apt-get install -y --no-install-recommends cmake uuid-dev pkg-config && \
    mkdir -p /opt/antlr4-cpp-runtime-4.7.1-source && cd /opt/antlr4-cpp-runtime-4.7.1-source && \
    wget http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-source.zip && \
    unzip antlr4-cpp-runtime-4.7.1-source.zip && \
    mkdir -p /opt/antlr4-cpp-runtime-4.7.1 && cd /opt/antlr4-cpp-runtime-4.7.1 && \
    cmake ../antlr4-cpp-runtime-4.7.1-source -DCMAKE_BUILD_TYPE=Debug && \
    make -j 2 && \
    make install

WORKDIR /development
