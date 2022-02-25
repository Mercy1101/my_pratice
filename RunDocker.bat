@echo off

set image_name=image_my_pratice
set container_name=container_my_pratice

Rem stop container
docker stop %container_name%

Rem del container
docker rm %container_name%

Rem del image
docker rmi %image_name%

Rem create image by Dockerfile
docker build -t %image_name% .

Rem create and run container
docker run -dt -m 1024m --restart=unless-stopped --privileged --name %container_name% %image_name%

Rem show all image
docker image ls

Rem show all container
docker ps -a

Rem enter running container bash
docker exec -it  %container_name% /bin/bash