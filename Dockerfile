# syntax=docker/dockerfile:1
FROM ubuntu:24.04

# Install dependencies.
RUN apt-get update && apt-get -y install cron && apt-get -y install g++ && apt-get -y install nano

# Setup cron job. For some reason, we have to copy the file because neither chmod nor
# crontab can find it otherwise.
COPY cronjob_benchmark cronjob
RUN chmod +x cronjob
RUN crontab cronjob

# Restart cron to ensure the job is running.
RUN /sbin/service cron start