# syntax=docker/dockerfile:1

# ���� Ubuntu 22.04
FROM ubuntu:22.04

# ���徲Ĭ��װ��������
ENV DEBIAN_FRONTEND noninteractive

# vim �����ļ�λ��
ENV VIM_CONFIG_FILE=/etc/vim/vimrc

# bashrc �ļ�λ��
ENV BASHEC_FILE=/root/.bashrc

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
    llvm \
    clang \
    gnutls-bin \
    re2c \
    curl \
    git-lfs \
    # У��ʱ��Ϊ�Ϻ�ʱ��
    && ln -snf /usr/share/zoneinfo/$TimeZone /etc/localtime && echo $TimeZone > /etc/timezone \
    && dpkg-reconfigure --frontend noninteractive tzdata \
    # ���� git
    && git config --global user.name lijiancong \
    && git config --global user.email lijiancong@gbcom.com.cn \
    && git config --global http.sslVerify false \
    && git config --global http.postBuffer 1048576000 \
    # && git config --global --unset http.proxy \
    # && git config --global --unset https.proxy \
    # ���� git log ��ʾ����
    && git config --global i18n.commitencoding utf-8 \
    && git config --global i18n.logoutputencoding utf-8 \
    && echo 'export LESSCHARSET=utf-8' >> ${BASHEC_FILE} \
    # ���� vim
    && echo 'set ts=4' >> ${VIM_CONFIG_FILE} \
    && echo 'set expandtab' >> ${VIM_CONFIG_FILE} \
    && echo 'set autoindent' >> ${VIM_CONFIG_FILE} \
    && echo 'set shiftwidth=4' >> ${VIM_CONFIG_FILE} \
    && echo 'set enc=utf8' >> ${VIM_CONFIG_FILE} \
    && echo 'set fencs=utf-8,gb2312,gb18030,gbk,ucs-bom,cp936,latin1' >> ${VIM_CONFIG_FILE} \
    && echo 'set fileencodings=ucs-bom,utf-8,utf-16,gbk,big5,gb18030,latin1e' >> ${VIM_CONFIG_FILE} \
    && echo 'set ff=unix' >> ${VIM_CONFIG_FILE}

# ����Ŀ¼
ENV WorkSpace=/root/workspace/
# github �ļ���
ENV GITHUB_PATH=${WorkSpace}/github

# ��װ ninja
RUN sudo apt-get install ninja-build
# RUN mkdir -p ${GITHUB_PATH} \
#     && git clone https://github.com/ninja-build/ninja.git ${GITHUB_PATH} \
#     && python3 ${GITHUB_PATH}/ninja/configure.py --bootstrap \
#     && cp ${GITHUB_PATH}/ninja /usr/bin

# oclint ������ļ�·��
ENV OCLINT_PATH=${GITHUB_PATH}/oclint-22.02
# oclint releases tar ������
ENV OCLINT_TAR_NAME=oclint-22.02-llvm-13.0.1-x86_64-linux-ubuntu-20.04.tar.gz

# ��װ oclint
RUN mkdir -p ${GITHUB_PATH} \
    && wget --no-check-certificate --content-disposition -P ${GITHUB_PATH}/ https://github.com/oclint/oclint/releases/download/v22.02/oclint-22.02-llvm-13.0.1-x86_64-linux-ubuntu-20.04.tar.gz \
    && mkdir -p ${OCLINT_PATH} \
    && tar -xzvf ${GITHUB_PATH}/${OCLINT_TAR_NAME} -C ${GITHUB_PATH} \
    && rm -f ${GITHUB_PATH}/${OCLINT_TAR_NAME} \
    && cp ${OCLINT_PATH}/bin/oclint* /usr/local/bin/ \
    && cp -rp ${OCLINT_PATH}/lib/* /usr/local/lib/

# ���ݿ�����
ENV MYSQL_ROOT_PASSWORD=123456
# mysql port
ENV MYSQL_PORT=3306
# mysql host
ENV MYSQL_LOCAL_HOST=localhost
# �����·��
ENV SOFTWARE_PATH=${WorkSpace}/sofware/
# mysql �����ļ�
ENV MYSQL_CONF_FILE=/etc/mysql/my.cnf

# ���ء���װ������ mysql5.7
# RUN mkdir -p ${SOFTWARE_PATH} \
#     && cd ${SOFTWARE_PATH} \
#     # ����������mysql 5.7��wget��,
#     && wget https://dev.mysql.com/get/Downloads/MySQL-5.7/mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
#     # ��ѹ
#     && tar -xvf mysql-server_5.7.31-1ubuntu18.04_amd64.deb-bundle.tar \
#     # ɾ�����ð�װ�İ�
#     && rm -f mysql-testsuite_5.7.31-1ubuntu18.04_amd64.deb mysql-community-test_5.7.30-1ubuntu18.04_amd64.deb mysql-community-test_5.7.31-1ubuntu18.04_amd64.deb \
#     # ���ý������ݣ�������װ�����еĽ�����������
#     && cat <<- DEBCONF| sudo debconf-set-selections \
#     mysql-server mysql-server/root_password_again password ${MYSQL_ROOT_PASSWORD} \
#     mysql-server mysql-server/root_password password ${MYSQL_ROOT_PASSWORD} \
#     DEBCONF \
#     # ��װ���а�װ��
#     && dpkg -i mysql-*.deb \
#     # ���mysql��������Ա�����Сд����������ipҲ�ɵ�¼
#     && echo '[mysqld]\nlower_case_table_names=1\nbind-address=0.0.0.0' >> ${MYSQL_CONF_FILE} \
#     # ���� mysql ����
#     && sudo service mysql start \
#     # �޸� root �˻��������Ȩ��
#     && sudo mysql -uroot -e "USE mysql; ALTER USER 'root'@'${MYSQL_LOCAL_HOST}' IDENTIFIED WITH mysql_native_password BY '${MYSQL_ROOT_PASSWORD}'; flush privileges;" \
#     # ʹ�� root �˻��������¼��ִ�д����û�������Ȩ�޵Ȳ���
#     && sudo mysql -h ${MYSQL_LOCAL_HOST} -P ${MYSQL_PORT} -uroot -p${MYSQL_ROOT_PASSWORD} -e "CREATE USER 'root'@'%' IDENTIFIED BY '123456';GRANT ALL ON *.* TO 'root'@'localhost';GRANT ALL ON *.* TO 'root'@'%';flush privileges;" \
#     # ��հ�װ��Ŀ¼
#     && rm -rf ${SOFTWARE_PATH}
#
# # coredump ��·��
# ENV CORE_DUMP_PATH=/root/coredump/
#
# # ���� coredump
# RUN ulimit -c unlimited \
#     # ���� coedump ��·��
#     && mkdir -p ${CORE_DUMP_PATH} \
#     # ���� coredump ���ɵ�·�����ļ���ʽ
#     && sysctl -w kernel.core_pattern=${CORE_DUMP_PATH}%e.%t.core

# ����Ŀ¼
ENV WORK_SPACE_PRATICE=${WorkSpace}/my_pratice

# �������ش���
COPY . ${WORK_SPACE_PRATICE}

# ���ù���·��
WORKDIR ${WORK_SPACE_PRATICE}