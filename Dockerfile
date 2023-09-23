FROM ubuntu:22.04

ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# System
RUN apt-get --yes -qq update \
    && apt-get --yes -qq upgrade \
    && apt-get --yes -qq install \
                      bzip2 \
                      cmake \
                      cpio \
                      curl \
                      g++ \
                      gcc \
                      gfortran \
                      git \
                      gosu \
                      libblas-dev \
                      liblapack-dev \
                      libopenmpi-dev \
                      openmpi-bin \
                      python3-dev \
                      python3-pip \
                      software-properties-common \
                      virtualenv \
                      wget \
                      zlib1g-dev \
                      emacs       \
                      htop      \
    && apt-get --yes -qq clean \
    && rm -rf /var/lib/apt/lists/*


RUN add-apt-repository universe \
    && apt-get --yes -qq update \
    && apt-get --yes -qq install graphviz

# Python
RUN python3 -m pip --no-cache-dir install --upgrade pip \
    && python3 -m pip --no-cache-dir install setuptools \
    && python3 -m pip --no-cache-dir install numpy \
    && python3 -m pip --no-cache-dir install h5py \
    && python3 -m pip --no-cache-dir install "dask[complete]" \
    && python3 -m pip --no-cache-dir install dask-jobqueue \
    && python3 -m pip --no-cache-dir install pydot \
    && python3 -m pip --no-cache-dir install pydot-ng \
    && python3 -m pip --no-cache-dir install graphviz

# Expose Dask Web UI
EXPOSE 8787

# Change user (not root)
ARG UNAME=gaelle
ARG UID=1000
ARG GID=1000
# GID, UID, UNAME can be overrided by --build-arg GID (ie) during building
RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME
USER $UNAME

# Avoid conflicts between CMA (Cross Memory Attach) and OpenMPI memory transport (vader) in the container
# See : https://github.com/open-mpi/ompi/issues/4948 
ENV OMPI_MCA_btl_vader_single_copy_mechanism=none

CMD [ "/bin/bash" ]
