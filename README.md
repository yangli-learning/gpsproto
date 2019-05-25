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

In the project root, create a build directory

    mkdir build;
    cd build;

Call cmake and make

    cmake ..;
    make;

Inside `build/`, call gpsproto on a test trajectory file:

    ./gpsproto ../test.pbf test_out.pbf

test.pbf and test_out.pbf should be the same.

### Notes

Each GPS sample can be annotated with the following attributes:
* `int32 car_id`: car/driver id 
* `uint32 timestamp`: linux time stamp of sample  
* `int32 lon`: longitude x 10^5
* `int32 lat`: latitude x 10^5
* `int32 head`: heading angle in degrees 
* `int32 speed`: speed in meter/second
* `float x`: easting in meters (e.g UTM coordinates)
* `float y`; northing in meters (e.g UTM coordinates) 


