@echo off

echo "Pushing updates to GitHub..."

git add -A
git commit -m "updates"
git push origin master

echo "done"