#include<iostream>
#include<sstream>
#include"leveldb\db.h"
#ifdef _DEBUG
#pragma comment(lib,"leveldbd.lib")
#else
#pragma comment(lib,"leveldb.lib")
#endif
int main() {
	std::cout << "leveldb version : " << leveldb::kMajorVersion << "." << leveldb::kMinorVersion << std::endl;
	
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	
	leveldb::Status status = leveldb::DB::Open(options, "testdb", &db);
	if (false == status.ok()) {
		std::cerr << "Unable to open/create test database './testdb'" << std::endl;
		std::cerr << status.ToString() << std::endl;
		return -1;
	}
	// Add 256 values to the database
	leveldb::WriteOptions writeOptions;
	for (unsigned int i = 0; i < 256; ++i) {
		std::ostringstream valueStream;
		valueStream << "Test data value : " << i;
		db->Put(writeOptions, std::to_string(i), valueStream.str());
	}
	// Iterate over each item in the database and print them
	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		std::cout << it->key().ToString() << " : " << it->value().ToString() << std::endl;
	}
	if (false == it->status().ok()) {
		std::cerr << "An error was found during the scan" << std::endl;
		std::cerr << it->status().ToString() << std::endl;
	}
	delete it;
	// Close the database
	delete db;
	return 0;
}