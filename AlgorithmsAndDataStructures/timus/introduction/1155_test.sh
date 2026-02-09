clang 1155.c -o 1155.out

printf "Test 1\n"
echo "1 0 1 0 3 1 0 0" | ./1155.out

printf "\n\nTest 2\n"
echo "0 1 0 1 2 3 2 2" | ./1155.out
