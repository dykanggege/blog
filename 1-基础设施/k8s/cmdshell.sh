#!/bin/bash

kubectl create secret docker-registry lingdong \
  --docker-server=mirrors.tencent.com \
  --docker-username=fukangding \
  --docker-password=220e34804ef411ea929b6c92bf47000d \
  --namespace=fukang

  