# syntax=docker/dockerfile:1

# ���� Ubuntu 22.04
FROM ubuntu:22.04

# ���徲Ĭ��װ��������
ENV DEBIAN_FRONTEND noninteractive

# vim �����ļ�λ��
ENV VIM_CONFIG_FILE=/etc/vim/vimrc

# ��װ���������
RUN apt-get update \
    && apt-get -y upgrade \
    && apt-get -y install \
    sudo \
    build-essential \
    git \
    gdb \
    uuid-dev \
    tzdata \
    net-tools \
    vim \
    wget \
    libmysqlclient-dev \
    psmisc \
    libjson-perl \
    libaio1 \
    libnuma1 \
    python3.9 \
    python-setuptools \
    libtinfo5 \
    libmecab2 \
    cmake \
    automake \
    apt-utils \
    manpages-de \
    manpages-de-dev \
    manpages-dev \
    glibc-doc \
    manpages-posix-dev \
    manpages-posix \
    && git config --global user.name lijiancong \
    && git config --global user.email pipinstall@163.com \
    && git config --global i18n.commitencoding utf-8 \
    && git config --global i18n.logoutputencoding utf-8 \
    && echo 'set ts=4\nset expandtab\nset autoindent\nset shiftwidth=4\nset enc=utf8\nset fencs=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1\nset fileencodings=ucs-bom,utf-8,utf-16,gbk,big5,gb18030,latin1e' >> ${VIM_CONFIG_FILE}

# У��ʱ��Ϊ�Ϻ�ʱ��
ENV TimeZone=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TimeZone /etc/localtime && echo $TimeZone > /etc/timezone && \
    dpkg-reconfigure --frontend noninteractive tzdata

# coredump ��·��
ENV CORE_DUMP_PATH=/root/coredump/

# ִ�б��벢���� coredump
RUN ulimit -c unlimited \
    && mkdir -p ${CORE_DUMP_PATH} \
    && sysctl -w kernel.core_pattern=${CORE_DUMP_PATH}%e.%t.core

# bashrc �ļ�λ��
ENV BASHEC_FILE=/root/.bashrc

# ���±�������� MySQL �ű�
ENV RECOMPIL_AND_START_MYSQL_SHELL_FILE=/root/cp4/recompile.sh

# ���� ���±��������MySQL�ű�
RUN echo 'export LESSCHARSET=utf-8' >> ${BASHEC_FILE}
