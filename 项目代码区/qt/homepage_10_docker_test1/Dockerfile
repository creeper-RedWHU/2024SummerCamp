FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install build-essential -y
RUN apt-get install qt6-base-dev -y
RUN apt-get install qt6-base-dev-tools
RUN apt-get install qt6-charts-dev -y
RUN apt-get install libmysqlclient-dev -y
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN qmake6 -o Makefile hemopage.pro
RUN make

EXPOSE 8070

CMD ["./hemopage"]