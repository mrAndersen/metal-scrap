docker build . -t mrandersen7/metal \
  && docker rm -f metal || true \
  && docker create --name metal --network host -v /proc:/hostproc --privileged metal_image \
  && docker start metal