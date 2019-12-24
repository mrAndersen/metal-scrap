docker build . -t metal_image \
  && docker rm -f metal || true \
  && docker create --name metal --network host -v /proc:/hostproc --privileged metal_image \
  && docker start metal \
  && docker exec -it metal bash