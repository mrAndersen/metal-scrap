version=$1

if [[ -z "$version" ]]; then
    echo -e "Enter version"
    exit 1
fi

docker build . -t mrandersen7/metal:"${version}" \
  && docker push mrandersen7/metal:"${version}"