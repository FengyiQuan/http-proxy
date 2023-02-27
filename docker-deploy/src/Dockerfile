FROM ubuntu:latest
RUN apt-get update
RUN apt-get install -y g++ make
RUN mkdir /var/log/erss
RUN touch /var/log/erss/proxy.log
RUN chmod 777 /var/log/erss
WORKDIR /proxy
ADD . /proxy/
# RUN cd proxy
RUN chmod +x run.sh