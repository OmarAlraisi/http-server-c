#!/bin/bash

curl localhost:8080 &
sleep 0.001
curl localhost:8080 &
sleep 0.001
curl localhost:8080 &
sleep 0.001
curl localhost:8080 &
sleep 0.001
curl localhost:8080 &

wait $!
