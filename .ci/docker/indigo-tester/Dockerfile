FROM debian:stable-slim

ARG JDK_VERSION_MAJOR=8u272
ARG JDK_VERSION_MINOR=b10
ARG MAVEN_VERSION=3.6.3
ARG JYTHON_VERSION=2.7.2
ARG DOTNET_VERSION=2.1
ARG IRONPYTHON_VERSION=2.7.10

RUN apt update && \
    apt upgrade -y && \
    apt install -y --no-install-recommends git git-lfs python3 python3-pip python3-distutils curl libfreetype6 libfontconfig1 unzip libc6-dev libgdiplus libx11-dev && \
    apt install -y gnupg && \
    # Java JDK
    curl -OL https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk${JDK_VERSION_MAJOR}-${JDK_VERSION_MINOR}/OpenJDK8U-jdk_x64_linux_hotspot_${JDK_VERSION_MAJOR}${JDK_VERSION_MINOR}.tar.gz && \
    tar -xzf OpenJDK8U-jdk_x64_linux_hotspot_${JDK_VERSION_MAJOR}${JDK_VERSION_MINOR}.tar.gz && \
    mv jdk${JDK_VERSION_MAJOR}-${JDK_VERSION_MINOR} /opt/jdk && \
    rm OpenJDK8U-jdk_x64_linux_hotspot_${JDK_VERSION_MAJOR}${JDK_VERSION_MINOR}.tar.gz && \
    # Maven
    curl -OL http://mirror.linux-ia64.org/apache/maven/maven-3/${MAVEN_VERSION}/binaries/apache-maven-${MAVEN_VERSION}-bin.tar.gz && \
    tar -xzf apache-maven-${MAVEN_VERSION}-bin.tar.gz && \
    mv apache-maven-${MAVEN_VERSION} /opt/maven && \
    rm apache-maven-${MAVEN_VERSION}-bin.tar.gz && \
    # Jython
    curl -OL https://repo1.maven.org/maven2/org/python/jython-installer/${JYTHON_VERSION}/jython-installer-${JYTHON_VERSION}.jar && \
    /opt/jdk/bin/java -jar jython-installer-${JYTHON_VERSION}.jar -s -d /opt/jython && \
    rm jython-installer-${JYTHON_VERSION}.jar && \
    # .NET Core
    curl -L https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.asc.gpg && \
    mv microsoft.asc.gpg /etc/apt/trusted.gpg.d/ && \
    curl -OL https://packages.microsoft.com/config/debian/10/prod.list && \
    mv prod.list /etc/apt/sources.list.d/microsoft-prod.list && \
    chown root:root /etc/apt/trusted.gpg.d/microsoft.asc.gpg && \
    chown root:root /etc/apt/sources.list.d/microsoft-prod.list && \
    apt update && \
    apt install -y powershell dotnet-sdk-${DOTNET_VERSION} && \
    ln -fs /usr/bin/pwsh /usr/bin/powershell && \
    # IronPython
    curl -OL https://github.com/IronLanguages/ironpython2/releases/download/ipy-${IRONPYTHON_VERSION}/IronPython.${IRONPYTHON_VERSION}.zip && \
    mkdir ironpython && \
    unzip IronPython.${IRONPYTHON_VERSION}.zip -d ./ironpython || true && \
    mv ironpython /opt/ && \
    chmod +x /opt/ironpython/netcoreapp${DOTNET_VERSION}/ipy.sh && \
    sed 's|$(dirname $0)|$(dirname $(readlink $0))|g' /opt/ironpython/netcoreapp${DOTNET_VERSION}/ipy.sh -i && \
    ln -fs /opt/ironpython/netcoreapp${DOTNET_VERSION}/ipy.sh  /opt/ironpython/ipy && \
    rm IronPython.${IRONPYTHON_VERSION}.zip && \
    # Python 3 Pillow
    python3 -m pip install --upgrade Pillow pip setuptools wheel twine && \
    apt autoremove -y

ENV JAVA_HOME=/opt/jdk
ENV PATH=${PATH}:${JAVA_HOME}/bin:/opt/jython/bin:/opt/maven/bin:/opt/ironpython

# Check
RUN git --version && \
    python3 --version && \
    twine --version && \
    wheel version && \
    java -version && \
    mvn --version && \
    gpg --version && \
    jython --version && \
    dotnet --version && \
    dotnet nuget --version && \
    ipy -V && \
    powershell --version
