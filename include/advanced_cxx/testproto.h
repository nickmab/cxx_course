#ifndef ADVANCED_CXX_TESTPROTO_H
#define ADVANCED_CXX_TESTPROTO_H

#include <advanced_cxx/fractal.pb.h>

// forward decl the proto msgs defined "fractal.pb.h"
namespace fractal_proto {
	class RGB;
}

namespace mabz {

fractal_proto::RGB protoget();
void strungout(const char* outfilename);

} /* namespace mabz */

/*

Finally, create a new MSVC solution & project for the example, and add add_person.cc, addressbook.pb.h and addressbook.pb.cc as source files. Set the build to Release. In the project's properties:

Add an additional include path, pointing to the full path of vsprojects/include
Add an additional library directory, pointing to the full path of vsprojects/Release
Add libprotobuf.lib as an additional dependency

 -- Install configuration: "Release"
  -- Installing: C:/Program Files (x86)/protobuf/lib/libprotobuf-lite.lib
  -- Installing: C:/Program Files (x86)/protobuf/lib/libprotobuf.lib
  -- Installing: C:/Program Files (x86)/protobuf/lib/libprotoc.lib
  -- Installing: C:/Program Files (x86)/protobuf/bin/protoc.exe
  -- Installing: C:/Program Files (x86)/protobuf/lib/pkgconfig/protobuf.pc
  -- Installing: C:/Program Files (x86)/protobuf/lib/pkgconfig/protobuf-lite.pc
  -- Installing: C:/Program Files (x86)/protobuf/include/google/protobuf/any.h
  -- Installing: C:/Program Files (x86)/protobuf/include/google/protobuf/any.pb.h
  */

#endif /* ADVANCED_CXX_TESTPROTO_H */