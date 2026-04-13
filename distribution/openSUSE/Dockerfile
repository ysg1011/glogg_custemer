FROM	opensuse/tumbleweed
RUN zypper --non-interactive --gpg-auto-import-keys ref && \
    zypper --non-interactive install --allow-downgrade \
    qt6-core-devel \
    qt6-widgets-devel \
    qt6-network-devel \
    qt6-serialport-devel \
    git-core \ 
    python \ 
    clang
