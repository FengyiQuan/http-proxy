FROM ubuntu:20.04
RUN apt-get update && apt-get install -y g++ make && apt-get install -y --no-install-recommends apt-utils
# RUN apt-get install -y g++ make
RUN mkdir /var/log/erss
RUN touch /var/log/erss/proxy.log
RUN chmod 777 /var/log/erss
WORKDIR /proxy
ADD . /proxy/
# RUN cd proxy
# EXPOSE 12345
RUN chmod +x run.sh