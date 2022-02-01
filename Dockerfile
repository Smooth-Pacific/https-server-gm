FROM ubuntu:20.04

# Set env to avoid user input interruption during installation
ENV TZ=America/Seattle
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install normal goodies
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends ssh \
                                               sudo \
                                               zsh \
                                               zsh-autosuggestions \
                                               git \
                                               curl \
                                               wget \
                                               vim \
                                               tree \
                                               zip \
                                               unzip \
                                               build-essential \
                                               pkg-config \
                                               clang \
                                               gdb \
                                               cscope \
                                               python3-dev \
                                               htop \
                                               iftop \
                                               iotop

# Change login shell to zsh
RUN chsh -s /bin/zsh $(whoami)

# Copy profile
COPY .profile /root/.profile
COPY .vimrc /root/.vimrc
COPY .zshrc /root/.zshrc

# TODO mount filesystem

# Spew nonsense
CMD ["echo", "run './root/setup.sh --all' setup the base environment"]