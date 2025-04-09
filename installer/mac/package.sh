#!/bin/bash

pkgbuild --root "$1" --identifier com.freod-sdk.freod --scripts "installer/mac/Scripts" --install-location "/tmp/freod-install" $2
