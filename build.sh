function compile_test {
  gcc -g druid.c -o druid -Wall -pedantic -D_TEST
}

function compile_app {
  gcc -g druid.c -o druid -Wall -pedantic -Werror
}

case "$1" in
  "test")
    compile_test ;;
  "app")
    compile_app  ;;
  *)
    echo "Usage: ./build.sh [test|app]"
esac

