# syntax=docker/dockerfile:1

# 基于 Ubuntu 22.04
FROM ubuntu:22.04

# 定义静默安装环境变量
ENV DEBIAN_FRONTEND noninteractive

# vim 配置文件位置
ENV VIM_CONFIG_FILE=/etc/vim/vimrc

# bashrc 文件位置
ENV BASHEC_FILE=/root/.bashrc

# 安装与配置软件
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
    llvm \
    clang \
    gnutls-bin \
    re2c \
    curl \
    git-lfs \
    # 校对时区为上海时区
    && ln -snf /usr/share/zoneinfo/$TimeZone /etc/localtime && echo $TimeZone > /etc/timezone \
    && dpkg-reconfigure --frontend noninteractive tzdata \
    # 配置 git
    && git config --global user.name lijiancong \
    && git config --global user.email lijiancong@gbcom.com.cn \
    && git config --global http.sslVerify false \
    && git config --global http.postBuffer 1048576000 \
    # && git config --global --unset http.proxy \
    # && git config --global --unset https.proxy \
    # 配置 git log 显示中文
    && git config --global i18n.commitencoding utf-8 \
    && git config --global i18n.logoutputencoding utf-8 \
    && echo 'export LESSCHARSET=utf-8' >> ${BASHEC_FILE} \
    # 配置 vim
    && echo 'set ts=4' >> ${VIM_CONFIG_FILE} \
    && echo 'set expandtab' >> ${VIM_CONFIG_FILE} \
    && echo 'set autoindent' >> ${VIM_CONFIG_FILE} \
    && echo 'set shiftwidth=4' >> ${VIM_CONFIG_FILE} \
    && echo 'set enc=utf8' >> ${VIM_CONFIG_FILE} \
    && echo 'set fencs=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1' >> ${VIM_CONFIG_FILE} \
    && echo 'set fileencodings=ucs-bom,utf-8,utf-16,gbk,big5,gb18030,latin1e' >> ${VIM_CONFIG_FILE} \
    && echo 'set ff=unix' >> ${VIM_CONFIG_FILE}

# 工作目录
ENV WorkSpace=/root/workspace/
# github 文件夹
ENV GITHUB_PATH=${WorkSpace}/github

# 安装 ninja
RUN sudo apt-get install ninja-build
# RUN mkdir -p ${GITHUB_PATH} \
#     && git clone https://github.com/ninja-build/ninja.git ${GITHUB_PATH} \
#     && python3 ${GITHUB_PATH}/ninja/configure.py --bootstrap \
#     && cp ${GITHUB_PATH}/ninja /usr/bin

# oclint 软件的文件路径
ENV OCLINT_PATH=${GITHUB_PATH}/oclint-22.02
# oclint releases tar 包名称
ENV OCLINT_TAR_NAME=oclint-22.02-llvm-13.0.1-x86_64-linux-ubuntu-20.04.tar.gz

# 安装 oclint
RUN mkdir -p ${GITHUB_PATH} \
    && wget --no-check-certificate --content-disposition -P ${GITHUB_PATH}/ https://github.com/oclint/oclint/releases/download/v22.02/oclint-22.02-llvm-13.0.1-x86_64-linux-ubuntu-20.04.tar.gz \
    && mkdir -p ${OCLINT_PATH} \
    && tar -xzvf ${GITHUB_PATH}/${OCLINT_TAR_NAME} -C ${GITHUB_PATH} \
    && rm -f ${GITHUB_PATH}/${OCLINT_TAR_NAME} \
    && cp ${OCLINT_PATH}/bin/oclint* /usr/local/bin/ \
    && cp -rp ${OCLINT_PATH}/lib/* /usr/local/lib/

# 数据库密码
ENV MYSQL_ROOT_PASSWORD=123456
# mysql port
ENV MYSQL_PORT=3306
# mysql host
ENV MYSQL_LOCAL_HOST=localhost
# 软件包路径
ENV SOFTWARE_PATH=${WorkSpace}/sofware/
# mysql 配置文件
ENV MYSQL_CONF_FILE=/etc/mysql/my.cnf

# 下载、安装和配置 mysql5.7
# RUN mkdir -p ${SOFTWARE_PATH} \
#     && cd ${SOFTWARE_PATH} \
#     # 从网上下载mysql 5.7的wget包,
#     && wget https://dev.mysql.com/get/Downloads/MySQL-5.7/mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
#     # 解压
#     && tar -xvf mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
#     # 删除不用安装的包
#     && rm -f mysql-testsuite_5.7.31-1ubuntu18.04_amd64.deb mysql-community-test_5.7.30-1ubuntu18.04_amd64.deb mysql-community-test_5.7.31-1ubuntu18.04_amd64.deb \
#     # 设置交互内容，跳过安装过程中的交互输入密码
#     && cat <<- DEBCONF| sudo debconf-set-selections \
#     mysql-server mysql-server/root_password_again password ${MYSQL_ROOT_PASSWORD} \
#     mysql-server mysql-server/root_password password ${MYSQL_ROOT_PASSWORD} \
#     DEBCONF \
#     # 安装所有安装包
#     && dpkg -i mysql-*.deb \
#     # 添加mysql配置项，忽略表名大小写，设置其他ip也可登录
#     && echo '[mysqld]\nlower_case_table_names=1\nbind-address=0.0.0.0' >> ${MYSQL_CONF_FILE} \
#     # 启动 mysql 服务
#     && sudo service mysql start \
#     # 修改 root 账户的密码和权限
#     && sudo mysql -uroot -e "USE mysql; ALTER USER 'root'@'${MYSQL_LOCAL_HOST}' IDENTIFIED WITH mysql_native_password BY '${MYSQL_ROOT_PASSWORD}'; flush privileges;" \
#     # 使用 root 账户和密码登录，执行创建用户和提升权限等操作
#     && sudo mysql -h ${MYSQL_LOCAL_HOST} -P ${MYSQL_PORT} -uroot -p${MYSQL_ROOT_PASSWORD} -e "CREATE USER 'root'@'%' IDENTIFIED BY '123456';GRANT ALL ON *.* TO 'root'@'localhost';GRANT ALL ON *.* TO 'root'@'%';flush privileges;" \
#     # 清空安装包目录
#     && rm -rf ${SOFTWARE_PATH}
#
# # coredump 的路径
# ENV CORE_DUMP_PATH=/root/coredump/
#
# # 设置 coredump
# RUN ulimit -c unlimited \
#     # 创建 coedump 的路径
#     && mkdir -p ${CORE_DUMP_PATH} \
#     # 配置 coredump 生成的路径和文件格式
#     && sysctl -w kernel.core_pattern=${CORE_DUMP_PATH}%e.%t.core

# 工作目录
ENV WORK_SPACE_PRATICE=${WorkSpace}/my_pratice

# 拷贝本地代码
COPY . ${WORK_SPACE_PRATICE}

# 设置工作路径
WORKDIR ${WORK_SPACE_PRATICE}