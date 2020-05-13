<<<<<<< HEAD
function compile_test {
  gcc -g druid.c -o test -D_TEST
}

function compile_app {
  gcc -g druid.c -o druid
}

case "$1" in
  "test")
    compile_test ;;
  "app")
    compile_app  ;;
  *)
    echo "Usage: ./build.sh [test|app]"
esac

=======
gcc druid.c -Wall -Werror -o druid
>>>>>>> preprocessor
