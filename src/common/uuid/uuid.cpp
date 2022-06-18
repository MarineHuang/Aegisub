
#include "uuid.h"
#ifdef _WIN32
#include <Windows.h>
#include <Rpc.h>
#include <wincrypt.h>
#else
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <uuid/uuid.h>
#endif

namespace SQ {

std::string GenerateUuid() {
#ifdef _WIN32
  char *data;
  UUID uuidhandle;
  UuidCreate(&uuidhandle);
  UuidToStringA(&uuidhandle, (RPC_CSTR*)&data);
  std::string uuid(data);
  RpcStringFreeA((RPC_CSTR*)&data);
  return uuid;
#else
  uuid_t uu;
  uuid_generate(uu);
  char buf[36];
  uuid_unparse(uu, buf);
  return buf;
#endif
}

}
