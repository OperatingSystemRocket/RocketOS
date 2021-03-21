FROM gcc:10.2.0

RUN apt-get -qq update

#TODO: move the contents of the bash script into Dockerfile and use RUN since then we get layer caching apparently
COPY toolchain.sh /toolchain.sh
RUN chmod +x /toolchain.sh
RUN /toolchain.sh
