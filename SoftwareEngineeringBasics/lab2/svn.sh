#!/bin/bash

COMMITS="../commits"
USER=red

# Разные пользователи, сахар
function red() {
  USER=red
}

function blue() {
  USER=blue
}

function commit() {
  # Очищаем всё, --force чтоб наверняка
  svn rm $(ls) --force
  # Добавляем новое состоеяни
  unzip ${COMMITS}/commit${1}.zip -d .
  svn add * --force
  svn commit -m "Revision: $1" --username $USER
}

function branch_from_trunk() {
  svn copy $REPO_URL/trunk $REPO_URL/branches/branch${1} -m "New branch: TRUNK -> branch$1" --username $USER
  switch $1
}


function branch() {
  svn copy $REPO_URL/branches/branch${1} $REPO_URL/branches/branch${2} -m "New branch: branch$1 -> branch$2" --username $USER
  switch $2
}

function switch() {
  svn switch $REPO_URL/branches/branch${1}
}

function switch_to_trunk() {
  svn switch $REPO_URL/trunk
}

function merge() {
  # Пытаемся смержить ветку в ветку
  # Цикл нужен, т.к. могут возникать несколько ошибок при одном мерже,
  # которые мы "разрешаем" интерактивно
  # Понятия не имею, как это ещё можно пофиксить, svn кал полнейший
  #
  # svn: E155015: One or more conflicts were produced while merging r8:10 into '/home/ghost/Desktop/ITMO/2nd_year/SoftwareEngineeringBasics/lab2/wd'
  # Вообще класс система контроля версий!
  while true; do
    # Вызываем мерж и помечаем, как отложенные, сохранем stderr
    output=$(svn merge --accept postpone $REPO_URL/branches/branch${1} 2>&1)
    echo "$output"
    # "Разрешаем"
    svn resolved . --depth=infinity 2>/dev/null || true
    # Прерываем цикл, если решили все проблемы при мерже
    if ! echo "$output" | grep -q "rerun the merge"; then
      break
    fi
  done

  commit $2
  ## 
  # svn rm $(ls) --force
  # unzip -o ${COMMITS}/commit${2}.zip -d .
  # svn resolved . --depth=infinity 2>/dev/null || true
  # svn add * --force
  # svn commit -m "Revision: $2" --username $USER
}


# Создание репо
svnadmin create repo
REPO_URL="file://$(pwd)/repo"

cd repo
svn mkdir -m "Lab 2 SVN" $REPO_URL/trunk $REPO_URL/branches
cd ..

svn checkout $REPO_URL/trunk/ wd
cd wd

# 0
unzip ${COMMITS}/commit0.zip -d .
svn add * --force
svn commit -m "INIT COMMIT" --username $USER

# 1
red
commit 1

# 2
blue
branch_from_trunk 2
commit 2

# 3
blue
branch 2 3
commit 3

# 4
red
switch_to_trunk
commit 4

# 5
red
branch_from_trunk 5
commit 5

# 6
blue
branch 5 6
commit 6

# 7
red
switch 5
commit 7

# 8
blue
switch 6
commit 8

# 9
blue
commit 9

# 10
blue
branch 6 10
commit 10

# 11
blue
switch 3
commit 11

# 12
blue
switch 2
commit 12

# 13
red
switch 5
commit 13

# 14
blue
switch 10
commit 14

# 15
blue
commit 15

# 16
red
switch_to_trunk
commit 16

# 17
blue
switch 10
commit 17

# 18
blue
switch 3
commit 18

# 19
red
switch 5
commit 19

# 20
red
commit 20

# 21
blue
switch 3
commit 21

# 22
blue
switch 2
commit 22

# 23
red
switch 5
merge 2 23

# 24
red
commit 24

# 25
red
commit 25

# 26
red
commit 26

# 27
red
switch_to_trunk
commit 27

# 28
blue
switch 10
commit 28

# 29
blue
switch 3
commit 29

# 30
blue
commit 30

# 31
blue
switch 6
commit 31

# 32
red
switch_to_trunk
commit 32

# 33
blue
switch 3
commit 33

# 34
blue
switch 6
merge 3 34

# 35
red
switch 5
merge 6 35

# 36
blue
switch 10
merge 5 36

# 37
red
switch_to_trunk
merge 10 37
