#! /usr/bin/env bash

PREFIX=$(dirname $0)
LIBS="${PREFIX}/lib"
DEST="${PREFIX}/bundle-libs"

LIBLOCATION="@executable_path/../Frameworks"

function printneval() {
    command="$@"
    echo "${command}"
    eval "${command}"
}

function printneval_sameline() {
    command="$@"
    printf "${command}:"
    eval "${command}" && echo
}

echo
echo "-- CLEAN --"
printneval rm -Rf "$DEST"

echo 
echo "-- REBUILD FROM LIBS --"
printneval cp -Rf "$LIBS" "$DEST"

echo
echo "-- MODFIY --"
for F in $(find "${DEST}" -type f -and -name "*.dylib"); do
    newid="${LIBLOCATION}/$(basename $F)"
    printneval_sameline install_name_tool -id "$newid" "$F"
done
