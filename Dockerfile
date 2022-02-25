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
    # 配置 git 用户名
    && git config --global user.name lijiancong \
    # 配置 git 邮箱
    && git config --global user.email pipinstall@163.com \
    # 配置 git log 支持显示汉字
    && git config --global i18n.commitencoding utf-8 \
    && git config --global i18n.logoutputencoding utf-8 \
    && echo 'export LESSCHARSET=utf-8' >> ${BASHEC_FILE} \
    # 配置 vim 的配置项
    && echo 'set ts=4\nset expandtab\nset autoindent\nset shiftwidth=4\nset enc=utf8\nset fencs=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1\nset fileencodings=ucs-bom,utf-8,utf-16,gbk,big5,gb18030,latin1e' >> ${VIM_CONFIG_FILE}

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
RUN mkdir -p ${SOFTWARE_PATH} \
    && cd ${SOFTWARE_PATH} \
    # 从网上下载mysql 5.7的wget包,
    && wget https://dev.mysql.com/get/Downloads/MySQL-5.7/mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
    # 解压
    && tar -xvf mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
    # 删除不用安装的包
    && rm -f mysql-testsuite_5.7.31-1ubuntu18.04_amd64.deb mysql-community-test_5.7.30-1ubuntu18.04_amd64.deb mysql-community-test_5.7.31-1ubuntu18.04_amd64.deb \
    # 设置交互内容，跳过安装过程中的交互输入密码
    && cat <<- DEBCONF| sudo debconf-set-selections \
    mysql-server mysql-server/root_password_again password ${MYSQL_ROOT_PASSWORD} \
    mysql-server mysql-server/root_password password ${MYSQL_ROOT_PASSWORD} \
    DEBCONF \
    # 安装所有安装包
    && dpkg -i mysql-*.deb \
    # 添加mysql配置项，忽略表名大小写，设置其他ip也可登录
    && echo '[mysqld]\nlower_case_table_names=1\nbind-address=0.0.0.0' >> ${MYSQL_CONF_FILE} \
    # 启动 mysql 服务
    && sudo service mysql start \
    # 修改 root 账户的密码和去那先
    && sudo mysql -uroot -e "USE mysql; ALTER USER 'root'@'${MYSQL_LOCAL_HOST}' IDENTIFIED WITH mysql_native_password BY '${MYSQL_ROOT_PASSWORD}'; flush privileges;" \
    # 使用 root 账户和密码登录，执行创建用户和提升权限等操作
    && sudo mysql -h ${MYSQL_LOCAL_HOST} -P ${MYSQL_PORT} -uroot -p${MYSQL_ROOT_PASSWORD} -e "CREATE USER 'root'@'%' IDENTIFIED BY '123456';GRANT ALL ON *.* TO 'root'@'localhost';GRANT ALL ON *.* TO 'root'@'%';flush privileges;" \
    # 清空安装包目录
    && rm -rf ${SOFTWARE_PATH}


# 校对时区为上海时区
ENV TimeZone=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TimeZone /etc/localtime && echo $TimeZone > /etc/timezone && \
    dpkg-reconfigure --frontend noninteractive tzdata

# coredump 的路径
ENV CORE_DUMP_PATH=/root/coredump/

# 执行编译并设置 coredump
RUN ulimit -c unlimited \
    && mkdir -p ${CORE_DUMP_PATH} \
    && sysctl -w kernel.core_pattern=${CORE_DUMP_PATH}%e.%t.core

ENV WORK_SPACE_PATH=/root/workspace/my_pratice

COPY . ${WORK_SPACE_PATH}

WORKDIR ${WORK_SPACE_PATH}