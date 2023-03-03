#!/bin/sh

echo "RUN_AFTER_CLONE.command"
echo "setting for local repository."

cd `dirname $0` # move to current directly
touch ./.git/hooks/pre-commit
cp -f  ./.gitlab/my_hooks/pre-commit.pl ./.git/hooks/pre-commit
chmod 755 ./.git/hooks/pre-commit
echo "copy finished."
echo "press ENTER to close window."
read a # require ENTER key push 