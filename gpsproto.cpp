/**
 * gpsproto I/O example: 
 * 
 * Usage: gpsproto INPUT_FILE OUTPUT_FILE 
 *
 * reads trajectories from pbf file into memory, then
 * writes back to a pbf file 
 * 
 * @author Yang Li 
 */
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <fcntl.h>
#include <unistd.h>
#include <memory>
#include "proto/gps_trajectory.pb.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;

typedef std::vector<std::shared_ptr<GpsTraj> > GpsTrajs;


bool readPBF(const char* pbfname, GpsTrajs &trajs){
	GOOGLE_PROTOBUF_VERIFY_VERSION;
	std::ifstream fin(pbfname, std::ios::binary);
	if (!fin.good()) {
		fprintf(stderr, "ERROR! Cannot open protobuf trajectory file %s\n",
				pbfname);
		return false;
	}
	google::protobuf::io::ZeroCopyInputStream *raw_input 
		= new google::protobuf::io::IstreamInputStream(&fin);
	google::protobuf::io::CodedInputStream *coded_input 
		= new google::protobuf::io::CodedInputStream(raw_input);
	coded_input->SetTotalBytesLimit(2000000000, 1000000000);

	uint32_t n_traj;
	if (!coded_input->ReadLittleEndian32(&n_traj)) {
		return false; // possibly end of file
	}
	
	cout << "reading " << n_traj <<" trajectories...\n"; 
	trajs.clear();
	for (size_t i = 0; i < n_traj; ++i) {
		uint32_t length;
		if (!coded_input->ReadLittleEndian32(&length)) {
			break; // possibly end of file
		}
		auto limit = coded_input->PushLimit(length);
		trajs.push_back(shared_ptr<GpsTraj>(new GpsTraj()));
		if (! trajs.back()->MergePartialFromCodedStream(coded_input)) {
			fprintf(stderr, "ERROR! Protobuf trajectory file contaminated!\n");
			trajs.pop_back();
			return false;
		} 			
		coded_input->PopLimit(limit);
	}
	fin.close();
	delete coded_input;
	delete raw_input;	
	return true;
}

void writePBF(const char* pbfname, GpsTrajs &trajs){

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	int fid = open(pbfname,O_WRONLY |O_CREAT |O_TRUNC );
	if (fid == -1){
		fprintf(stderr,"Error: cannot create protobuf file. \n");
		return;
	}
	google::protobuf::io::ZeroCopyOutputStream *raw_output 
		= new google::protobuf::io::FileOutputStream(fid);
	google::protobuf::io::CodedOutputStream *coded_output 
		= new google::protobuf::io::CodedOutputStream(raw_output);
	uint32_t num_trajectory = trajs.size();

	coded_output->WriteLittleEndian32(num_trajectory);  

	for (size_t i =0; i< num_trajectory; ++i){
		string s;
		trajs[i]->SerializeToString(&s);
		coded_output->WriteLittleEndian32(s.size());
		coded_output->WriteString(s);
	}
	delete coded_output;
	delete raw_output;
	close(fid);
	google::protobuf::ShutdownProtobufLibrary();
	cout <<"exported pbf file " << pbfname << endl;
}

void test(const GpsTrajs &trajs){
	if (!trajs.empty()){
		cout << "first point in trajectory 0 :"<<endl;
		shared_ptr<GpsTraj> traj = trajs[0];
		if (traj->point_size()>0){
			const TrajPoint p=traj->point(0);
			cout << " car id: " << p.car_id() 
				 << "\n timestamp: " << p.timestamp()
				 << "\n lon (x1e5): " << p.lon()
				 << "\n lat (x1e5): " << p.lat()
				 << "\n easting: " << p.x()
				 << "\n northing: " << p.y()
				 << endl;
				
		}
	}
}

void usage(){
	cout <<"Usage: gpsproto INPUT_FILE OUTPUT_FILE "<<endl <<endl;
}

int main(int argc, char** argv){
	if (argc < 3){
		usage();
		return 0;
	}	
	GpsTrajs trajs;

	if (readPBF(argv[1],trajs)){
		test(trajs);
		writePBF(argv[2],trajs);
	}

}


