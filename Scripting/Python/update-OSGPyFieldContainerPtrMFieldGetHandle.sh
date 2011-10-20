#!/bin/sh

# Usage: fcd2code [options]
#
# Options:
#  -h, --help            show this help message and exit
#  -d file.fcd, --fcd-file=file.fcd
#                        FieldContainer description file.
#  -b, --write-base      write the FieldContainerBase files. [default: false].
#  -f, --write-fc        write the FieldContainer files. [default: false].
#  -p PATH, --out-path=PATH
#                        destination path for files.
#  -r PATH, --root-path=PATH
#                        root of the OpenSG source tree.
#  -t PATH, --template-path=PATH
#                        path to where the fcd2code templates are stored.
#  -v, --verbose         print diagnostic messages. [default: false].
#  -c, --compat-mode     enable 1.x compatibility mode. [default: false].
#  -B, --bundle          create field bundle. [default: false].

FCDFILE=OSGPyFieldContainerPtrMFieldGetHandle.fcd
OUT_DIR=.
OSG_ROOT=~/dev/Libs/install/OpenSG
PYTHON=python
FCD2CODE_SCRIPT=$OSG_ROOT/bin/fcd2code/fcd2code
TEMPLATE_PATH=$OSG_ROOT/bin/fcd2code

$PYTHON $FCD2CODE_SCRIPT --fcd-file $FCDFILE -p $OUT_DIR -r $OSG_ROOT -t $TEMPLATE_PATH -b -f
