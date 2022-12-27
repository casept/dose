FROM docker.io/debian:11

RUN apt update && apt upgrade -y

RUN apt install -y clang-format

# Install MXE from source, as binary packages are outdated
RUN apt install -y \
	build-essential \
	git \
	autoconf \
	automake \
	autopoint \
	bash \
	bison \
	bzip2 \
	flex \
	g++ \
	g++-multilib \
	gettext \
	git \
	gperf \
	intltool \
	libc6-dev-i386 \
	libgdk-pixbuf2.0-dev \
	libltdl-dev \
	libssl-dev \
	libtool-bin \
	libxml-parser-perl \
	lzip \
	make \
	openssl \
	p7zip-full \
	patch \
	perl \
	python3 \
	python3-mako \
	ruby \
	sed \
	unzip \
	wget \
	xz-utils
RUN ln -s /usr/bin/python3 /usr/bin/python

WORKDIR /
RUN git clone https://github.com/mxe/mxe.git
WORKDIR /mxe
RUN git checkout 114b1ac37013122fdb77017eca324c315a93dd93
RUN make MXE_TARGETS='x86_64-w64-mingw32.static' cc

RUN apt install -y python3-pip
RUN pip install quom
