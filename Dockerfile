# syntax=docker/dockerfile:1
FROM ubuntu:24.04

# Install dependencies.
RUN apt-get update && apt-get -y install cron && apt-get -y install git && apt-get -y install nano

# Setup cron job.
USER ubuntu
WORKDIR /workspaces/nrec_benchmark
RUN chmod +x cronjob
RUN crontab cronjob