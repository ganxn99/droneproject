FROM  csci3081f20/base as env

RUN groupdel dialout

RUN apt-get update && apt-get install -y \
    python2.7\
    python-pip\
    build-essential \
    gdb \
    libssl-dev \
    zlib1g-dev \
    dos2unix \
    rsync \
    doxygen \
    graphviz \
    libc6-dbg \
    valgrind

RUN pip install cpplint
ENV project_port 8081
ENV USE_REPO_DIR 1

ARG USER_ID
ARG GROUP_ID

RUN addgroup --gid $GROUP_ID user
RUN adduser --disabled-password --gecos '' --uid $USER_ID --gid $GROUP_ID user

RUN mkdir -p /home/user
WORKDIR /home/user/repo

USER user
