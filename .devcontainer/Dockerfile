FROM ubuntu:latest
RUN apt-get -y update
RUN apt-get -y install git
RUN apt-get -y install vim
RUN apt-get -y install clang
RUN apt-get -y install guile-3.0
RUN apt-get -y install valgrind
RUN apt-get -y install gdb
RUN apt-get -y update
RUN apt-get -y install zip
# just is not available via apt;  it wants snap which doesn't work in docker containers so we are manually downloading/running the just script into /usr/local
RUN apt-get -y install curl
RUN curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to /usr/local/bin
# create a csuser for use rather than root
RUN useradd -ms /bin/bash csuser
USER csuser
