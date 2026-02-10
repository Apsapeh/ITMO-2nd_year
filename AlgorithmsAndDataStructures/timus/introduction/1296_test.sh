clang 1296.c -o 1296.out

printf "Test 1\n"
echo "10
31
-41
59
26
-53
58
97
-93
-23
84" | ./1296.out

printf "\n\nTest 2\n"
echo "3
-1
-5
-6" | ./1296.out
