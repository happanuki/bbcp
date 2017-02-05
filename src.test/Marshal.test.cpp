#include "gtest/gtest.h"
#include "Marshal.h"

class MarshalTest : public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {}

};


TEST_F(MarshalTest,Marshal_Unmarshal_Strings)
{
	std::string inputStr1 = "some test string with blabla/asdasdasxzc";
	std::string inputStr2 = "NVMNVM Hello world!@#";

	auto m1 = Marshal::marshalString(inputStr1);
	auto m2 = Marshal::marshalString(inputStr2);

	std::unique_ptr<char[]> marshalStream( new char[m1.size + m2.size] );
	char* ptr = marshalStream.get();

	memcpy( ptr, m1.ptr.get(), m1.size );
	memcpy( ptr + m1.size, m2.ptr.get(), m2.size );


	std::string resultStr1 = Marshal::unmarshalString(&ptr);
	std::string resultStr2 = Marshal::unmarshalString(&ptr);

	ASSERT_STREQ(inputStr1.c_str(),resultStr1.c_str());
	ASSERT_STREQ(inputStr2.c_str(),resultStr2.c_str());
}


TEST_F(MarshalTest,Marshal_Unmarshal_Offsets)
{

	Offsets_t ofs1 = { {1,2}, {3,4}, {5,6}, {7,8}, {9,0}, {11,12} };
	Offsets_t ofs2 = { {10,20}, {30,40}, {50,60}, {70,80}, {90,0} };

	auto m1 = Marshal::marshalOffsets(ofs1);
	auto m2 = Marshal::marshalOffsets(ofs2);

	std::unique_ptr<char[]> marshalStream( new char[m1.size + m2.size] );
	char* ptr = marshalStream.get();

	memcpy( ptr, m1.ptr.get(), m1.size );
	memcpy( ptr + m1.size, m2.ptr.get(), m2.size );

	Offsets_t rofs1 = Marshal::unmarshalOffsets(&ptr);
	Offsets_t rofs2 = Marshal::unmarshalOffsets(&ptr);

	for (auto i=0; i < ofs1.size(); ++i) {
		if (ofs1[i] != rofs1[i]) {
			ASSERT_TRUE(false);
		}
	}

	for (auto i=0; i < ofs2.size(); ++i) {
		if (ofs2[i] != rofs2[i]) {
			ASSERT_TRUE(false);
		}
	}
}


TEST_F(MarshalTest,Marshal_Unmarshal_FileInternals)
{

	Offsets_t ofs1 = { {1,2}, {3,4}, {5,6}, {7,8}, {9,0}, {11,12} };
	Offsets_t ofs2 = { {10,20}, {30,40}, {50,60}, {70,80}, {90,0} };

	FileInternals fi1;
	fi1.fileName = "justFilename1";
	fi1.offsets = ofs1;

	FileInternals fi2;
	fi2.fileName = "justFilenamexxx2";
	fi2.offsets = ofs2;

	auto m1 = Marshal::marshalFileInternals(fi1);
	auto m2 = Marshal::marshalFileInternals(fi2);

	std::unique_ptr<char[]> marshalStream( new char[m1.size + m2.size] );
	char* ptr = marshalStream.get();

	memcpy( ptr, m1.ptr.get(), m1.size );
	memcpy( ptr + m1.size, m2.ptr.get(), m2.size );

	FileInternals rfi1 = Marshal::unmarshalFileInternals(&ptr);
	FileInternals rfi2 = Marshal::unmarshalFileInternals(&ptr);

	//checks

	ASSERT_STREQ(rfi1.fileName.c_str(),fi1.fileName.c_str());
	ASSERT_STREQ(rfi2.fileName.c_str(),fi2.fileName.c_str());

	for (auto i=0; i < rfi1.offsets.size(); ++i) {
		if (fi1.offsets[i] != rfi1.offsets[i]) {
			ASSERT_TRUE(false);
		}
	}

	for (auto i=0; i < rfi2.offsets.size(); ++i) {
		if (fi2.offsets[i] != rfi2.offsets[i]) {
			ASSERT_TRUE(false);
		}
	}
}
