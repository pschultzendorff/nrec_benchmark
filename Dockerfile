# syntax=docker/dockerfile:1
FROM python:3.12-slim

# Install dependencies.
RUN apt-get update && apt-get -y install cron && apt-get -y install g++ && apt-get -y install nano
WORKDIR /workspaces/nrec_benchmark

# Setup python. job. For some reason, we have to copy the file because pip cannot find
# it otherwise.
COPY requirements.txt requirements.txt
RUN pip install --upgrade pip
RUN pip install -r requirements.txt 

# Setup cron job. For some reason, we have to copy the file because neither chmod nor
# crontab can find it otherwise.
COPY cronjob_benchmark cronjob
RUN chmod +x cronjob
RUN crontab cronjob
RUN rm cronjob

# Start cron service.
CMD cron -f