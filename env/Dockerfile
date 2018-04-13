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

RUN echo oracle-java9-installer shared/accepted-oracle-license-v1-1 select true | debconf-set-selections && \
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys C2518248EEA14886 && \
    add-apt-repository ppa:webupd8team/java && \
    apt-get update && \
    apt-get install oracle-java9-installer -y && \
    java --version

RUN mkdir -p /opt/gradle && cd /opt/gradle && \
    wget https://downloads.gradle.org/distributions/gradle-4.6-bin.zip && \
    unzip gradle-4.6-bin.zip && \
    export PATH=$PATH:/opt/gradle/gradle-4.6/bin && \
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

WORKDIR /development
