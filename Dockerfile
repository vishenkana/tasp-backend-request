FROM tasp/libcurl:1.0.0 as curl
LABEL stage=builder

FROM tasp/libdb-pg:1.0.0 as db
LABEL stage=builder

FROM tasp/libmicroservice:1.0.0 as build
LABEL stage=builder

COPY . /requests
WORKDIR /requests

COPY --from=curl /curl/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=curl /usr/include/tasp /usr/include/tasp

COPY --from=db /db-pg/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=db /usr/include/tasp /usr/include/tasp

RUN mkdir build && cd build && cmake .. && ninja

FROM tasp/astra:1.7.5-smolensk

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && apt-get install -y --no-install-recommends --reinstall \
        gcc-astra-libs \
        libjsoncpp1 \
        libyaml-cpp0.6 \
        libevent-2.1-6 \
        libcurl4 \
        libpq5

COPY --from=build /common/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=build /microservice/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=curl /curl/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=db /db-pg/build/bin/libtasp*.so* /lib/x86_64-linux-gnu/
COPY --from=build /requests/build/bin/tasp-requests /usr/bin

COPY etc/tasp-requests-docker.yml /etc/tasp/tasp-requests.yml

STOPSIGNAL SIGINT

EXPOSE 80
CMD /usr/bin/tasp-requests
