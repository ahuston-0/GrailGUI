#include "csp/XDLRequest.hh"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <csp/csp.hh>
#include <cstdint>
#include <fstream>
#include <iostream>

//#include "util/TypeAlias.hh"
#include "xdl/SymbolTable.hh"
#include "xdl/XDLArray.hh"
#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"

using namespace std;

// base case
void buildData(Buffer& out, char meta[]) {}

// template <typename T, typename... Args>
// void buildData(Buffer& out, Buffer& meta, T first, const Args&... args) {

template <typename T>
void buildData2(ArrayOfBytes* a, const T& arg) {
  if (is_base_of_v<XDLType, arg>) {
    if (is_base_of_v<XDLBuiltinType, arg>) {
      a->addMeta(arg.getType());
      a->addData(arg);
    } else {
      if (is_base_of_v<CompoundType, arg>) {
        arg.addMeta(a);
        arg.addData(a);
      }
    }
  } else {
    a->addMeta(typeToDataType(arg));
    a->addData(arg);
  }
}

template <typename... Args>
void buildData(DynArray<XDLType*> a, const Args&... args) {
  ArrayOfBytes* bytes = new ArrayOfBytes();
  (buildData2(bytes, args), ...);
  a.add(bytes);
}
#if 0
template <std::size_t datasize, std::size_t metasize, typename... Args>
void buildData(std::array<uint8_t, size>& data, std::array<uint8_t, size>& meta, const Args&... args) {
  (out.write(args), out.writeMeta(args), ...);
}
#endif

class Point : public CompoundType {
 public:
  double x, y, z;
  Point(double x, double y, double z)
      : CompoundType("Point"), x(x), y(y), z(z) {}
  void write(Buffer& out) const override;

  DataType getDataType() const { return DataType::STRUCT8; }
  uint32_t size() const { return 12; }

  void addData(ArrayOfBytes* data) const {
    data->addData(x);
    data->addData(y);
    data->addData(z);
  }

  void addMeta(XDLArray* meta) const {
    meta->addStruct("Point", 3);
    meta->addBuiltinMember(DataType::F64, "x");
    meta->addBuiltinMember(DataType::F64, "y");
    meta->addBuiltinMember(DataType::F64, "z");
  }
};

void write(Buffer& out, const Point& p);

XDLRequest::XDLRequest(const char filename[]) : Request(), xdlData(3) {
  buildData(xdlData, Point(1, 2, 3));

  // Create list later
  // auto listonums = new List<uint32_t>(100);
  // for (int i = 0; i < 100; i++) listonums->add(i);
  // xdlData.add(listonums);

  auto pointList = new List<Point>(100);
  for (int i = 0; i < 100; i++) pointList->add(Point(i, 2, 3));
  xdlData.add(pointList);

  // TODO: create all the XDL objects to satisfy requests
  // TODO: for now, ignore the filename and hardcode some objects

  compiler = new XDLCompiler(filename);
  SymbolTable* st = new SymbolTable(compiler);

  // page 0
  // Metadata
  // STRUCT8 0"" 4 U32 x U64 y STRUCT8 5Point 3 F32 x F32 y F32 z 5point F64
  // 4bach
  //
  Struct* s = new Struct(compiler);  // name encoded as 0""
  s->addBuiltin("x", DataType::U32);
  s->addBuiltin("y", DataType::U64);
  Struct* s2 = new Struct(compiler, "Point");
  s2->addBuiltin("x", DataType::F32);
  s2->addBuiltin("y", DataType::F32);
  s2->addBuiltin("z", DataType::F32);
  s->addMember("point", s2);
  s->addBuiltin("bach", DataType::F64);

  // st->addRoot(s);
  // Buffer ans(32768);
  // ans.write(uint32_t(1));
  // ans.write(uint64_t(3));
  // ans.write(float(1.5));
  // ans.write(float(2.5));
  // ans.write(float(3.5));
  // ans.write(double(123.456));

  //  st->attachData(1, 2, {1, 2, 3}, 2.5);
  xdlData.add(s);

  // page 2
  // Metadata LIST8 ??? U32
  // st = new SymbolTable(compiler);
  // st->addRoot();
  xdlData.add(new GenericList(compiler, "fred", DataType::U32));

  // page4
  // Metadata LIST8 6mylist ??? STRUCT8 6Person 3 STRING8, 9firstname STRING8,
  // 8lastname, U32 3age
  // st = new SymbolTable(compiler);
  s = new Struct(compiler, "Person");
  s->addBuiltin("firstname", DataType::STRING8);
  s->addBuiltin("lastname", DataType::STRING8);
  s->addBuiltin("age", DataType::U32);
  // st->addRoot();
  xdlData.add(new GenericList(compiler, "people", s));

  // referring to an already-existing type
  // STRUCT8 3abc 2 LIST8 6mylist F64 LIST8 6mylist
  s = new Struct(compiler, "abc");
  GenericList* list;
  s->addMember("a",
               (list = new GenericList(compiler, "mylist", DataType::F64)));
  s->addMember("b", list);
  xdlData.add(s);

  //  List<StockQuote>* quotes = new List<StockQuote>();
  //  for (int i = 0; i < n; i++) quotes->add(new StockQuote(...));

  // xdlData.add(quotes);  // hardcoded class which should spew
  // Metadata LIST16 5Quotes STRUCT8 10StockQuote 6 F32 open F32 hi F32 LOW
  // F32

  // close ...

  // load page 1
  addPage("res/aapl.bin");
  //  List<StockQuote> quotes;

  //  out.write(quote);

#if 0
	List<Quote> *quotes = new List<StockQuote>();
  quotes.read("res/aapl.bin");
  s->addMember("quotes", quotes);
#endif

#if 0
  st = new SymbolTable(compiler);
  s = new Struct(compiler);
  s->addMember("uid", new U128((uint64_t)12345, (uint64_t)123456));
  st->addRoot(s);
  xdlData.add(st);

  st = new SymbolTable(compiler);
  st->addRoot(new ContactInfo(Address("90 W. 123 St. New York, NY 10027"),
                                 Phone(2125512638), Email("test@yahoo.com")));
  xdlData.add(st);

  // this is just metadata, no actual values (of limited utility hardcoded in a
  // server)
  st = new SymbolTable(compiler);
  // TODO: st = new SymbolTable(compiler, "fred", "DataType::U32", "alice",
  // DataType::STRING8);
  // Using template<typename... Args> (variatic template pair parameters) ---
  s = new Struct(compiler);
  s->addBuiltin("fred", DataType::U32);
  s->addBuiltin("alice", DataType::STRING8);
  st->addRoot(s);

  // Replacing this ^--------------------------------------------------------
  xdlData.add(st);
#endif
}

void XDLRequest::handle(int fd) {
  cout << "handling XDL request" << endl;
  out.attachWrite(fd);
  in.attachRead(fd);
  in.displayRawRead();
  // buffer ..   buffer+dataSize
  // for now, hardcoded first 4 bytes of buffer is the request number
  uint32_t requestId = in.readU32();
  cout << "requestId: " << requestId << '\n';
  if (requestId >= xdlData.size()) {
    // srvlog.error(Errcode::ILLEGAL_SERVLETID);
    // commented this line out because it causes an error
    // ERROR: In function `CSPRequest::handle(int)':
    // CSPRequest.cc:(.text+0xf6): undefined reference to `srvlog'
    cout << "ERROR::ILLEGAL_SERVLETID\n";
    return;
  }

  XDLType* x = xdlData[requestId];
  // Struct* s = (Struct*)st->getSymbol(root);
  x->writeMeta(out);
  x->write(out);
  out.displayRaw();
  out.flush();
}

XDLRequest::~XDLRequest() {
  delete compiler;
  for (int i = 0; i < xdlData.size(); i++) {
    delete xdlData[i];
  }
}
void XDLRequest::handle(int, char const*) {}

void XDLRequest::addPage(const char metaDataFilename[], const char filename[]) {
}
#if 1
void XDLRequest::addPage(const char filename[]) {
  int fh = open(filename, readBinFlags);
  if (fh < 0) {
    cerr << "Error opening file " << filename << '\n';
    throw Ex1(Errcode::FILE_NOT_FOUND);
  }
  struct stat s;
  fstat(fh, &s);
  // create an XDLRaw object with all the data in the file
  char* p = new char[s.st_size];
  ssize_t bytesRead = read(fh, p, s.st_size);
  if (bytesRead < s.st_size) {
    cerr << "Error reading file " << filename << '\n';
  }
  xdlData.add(new XDLRaw(p, s.st_size));
  close(fh);
}
#endif

// TODO: Verify that above implementation works on both Linux and Windows.
//  If so, delete below implementation of addPage
#if 0
void XDLRequest::addPage(const char filename[]) {
	ifstream in(filename, ios::binary| ios::ate);
  if (!in.good()) {
		cerr << "Error opening file " << filename << '\n';
    return;
  }
  size_t size = in.tellg();
	// create an XDLRaw object with all the data in the file
	char* p = new char[size];
	in.read(p, size);
	xdlData.add(new XDLRaw(p, size));
}
#endif