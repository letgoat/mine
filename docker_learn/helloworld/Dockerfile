# syntax=docker/dockerfile:1
FROM ubuntu:22.04

#install app dependencies
RUN apt-get update && apt-get install -y python3 python3-pip
RUN pip install flask==3.0.*

#create working directory
WORKDIR /app

#install app
COPY hello.py /app/

#final configuration
EXPOSE 8000
CMD ["python3", "hello.py"]
