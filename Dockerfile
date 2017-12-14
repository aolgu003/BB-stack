FROM amd64/ubuntu:16.04

COPY ./setup_env.sh /

RUN chmod +x setup_env.sh
RUN ./setup_env.sh
