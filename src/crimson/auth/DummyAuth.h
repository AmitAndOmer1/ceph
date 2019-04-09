// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include "AuthClient.h"
#include "AuthServer.h"

namespace ceph::auth {

class DummyAuthClientServer : public AuthClient,
                              public AuthServer {
public:
  DummyAuthClientServer() {}

  // client
  std::pair<std::vector<uint32_t>, std::vector<uint32_t>>
  get_supported_auth_methods(int peer_type) final {
    return {{CEPH_AUTH_NONE}, {CEPH_AUTH_NONE}};
  }

  uint32_t pick_con_mode(int peer_type,
			 uint32_t auth_method,
			 const std::vector<uint32_t>& preferred_modes) final {
    ceph_assert(auth_method == CEPH_AUTH_NONE);
    ceph_assert(preferred_modes.size() &&
                preferred_modes[0] == CEPH_CON_MODE_CRC);
    return CEPH_CON_MODE_CRC;
  }

  AuthAuthorizeHandler* get_auth_authorize_handler(int peer_type,
						   int auth_method) final {
    return nullptr;
  }

  int get_auth_request(
    ceph::net::ConnectionRef conn,
    AuthConnectionMetaRef auth_meta,
    uint32_t *method,
    std::vector<uint32_t> *preferred_modes,
    bufferlist *out) override {
    *method = CEPH_AUTH_NONE;
    *preferred_modes = { CEPH_CON_MODE_CRC };
    return 0;
  }

  int handle_auth_reply_more(
    ceph::net::ConnectionRef conn,
    AuthConnectionMetaRef auth_meta,
    const bufferlist& bl,
    bufferlist *reply) override {
    ceph_abort();
  }

  int handle_auth_done(
    ceph::net::ConnectionRef conn,
    AuthConnectionMetaRef auth_meta,
    uint64_t global_id,
    uint32_t con_mode,
    const bufferlist& bl,
    CryptoKey *session_key,
    std::string *connection_secret) {
    return 0;
  }

  int handle_auth_bad_method(
    ceph::net::ConnectionRef conn,
    AuthConnectionMetaRef auth_meta,
    uint32_t old_auth_method,
    int result,
    const std::vector<uint32_t>& allowed_methods,
    const std::vector<uint32_t>& allowed_modes) override {
    ceph_abort();
  }

  // server
  int handle_auth_request(
    ceph::net::ConnectionRef conn,
    AuthConnectionMetaRef auth_meta,
    bool more,
    uint32_t auth_method,
    const bufferlist& bl,
    bufferlist *reply) override {
    return 1;
  }
};

} // namespace ceph::auth
