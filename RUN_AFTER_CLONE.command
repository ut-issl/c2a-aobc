#!/bin/sh

function make_link () {
  if [ -e $1 ]; then
    rm -rf $1
  fi
  ln -s $2 $1
}

echo "RUN_AFTER_CLONE.command"
echo "setting for local repository."

echo "[SET UP DEVELOP ENVIRONMENT FOR SAMPLE AOBC USER]"
make_link "./Examples/src/src_aobc" "./"
echo done.
echo.

cd `dirname $0` # move to current directly
touch ./.git/hooks/pre-commit
cp -f  ./.gitlab/my_hooks/pre-commit.pl ./.git/hooks/pre-commit
chmod 755 ./.git/hooks/pre-commit
echo "copy finished."
echo "press ENTER to close window."
read a # require ENTER key push 