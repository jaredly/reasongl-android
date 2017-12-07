set -ex
./build.sh
cd example-project/ && ./gradlew installDebug
