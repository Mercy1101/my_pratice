@echo off
echo "stop all container"
FOR /f "tokens=*" %%i IN ('docker ps -q') DO docker stop %%i
echo "delete all container"
FOR /f "tokens=*" %%i IN ('docker ps -aq') DO docker rm %%i
echo "delete all image"
FOR /f "tokens=*" %%i IN ('docker images --format "{{.ID}}"') DO docker rmi %%i