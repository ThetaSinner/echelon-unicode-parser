
apt update
apt upgrade

# TODO Some of these libraries have newer versions, check for build compatibility next time this script is run.
apt install -y build-essential libssl-dev zlib1g-dev libncurses5-dev libncursesw5-dev libreadline-dev libsqlite3-dev libgdbm-dev libdb5.3-dev libbz2-dev libexpat1-dev liblzma-dev tk-dev libffi-dev

mkdir -p /opt/python && cd /opt/python
wget https://www.python.org/ftp/python/3.7.0/Python-3.7.0.tar.xz
tar xf Python-3.7.0.tar.xz
cd Python-3.7.0
./configure --enable-optimizations
make -j 8
make altinstall
