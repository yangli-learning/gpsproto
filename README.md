The gps_trajectory.proto Format
-------------------------------------------------------
by [Chen Chen](http://github.com/cchen1986) , [Yang Li](http://github.com/yangli1-stanford)

### Dependency ###

Google Protocol Buffers

  <https://developers.google.com/protocol-buffers>


### Compile protobuf class ###

   Go to `proto/`, compile the format file `gps_trajectory.proto`

    protoc -I=. --cpp_out=. gps_trajectory.proto

   This will generate `proto/gps_trajectory.pb.h` and `proto/gps_trajectory.pb.cc`


### Compile & run example ###

Create a build directory

    mkdir build;
    cd build;

Call cmake and make

    cmake ..;
    make;

Inside `build/`, call gpsproto on a test trajectory file:

    ./gpsproto ../test.pbf test_out.pbf

test.pbf and test_out.pbf should be the same.


