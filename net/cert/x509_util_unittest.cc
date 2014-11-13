// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/cert/x509_util.h"

#include <algorithm>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "crypto/rsa_private_key.h"
#include "net/cert/x509_certificate.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace net {

namespace x509_util {

TEST(X509UtilTest, SortClientCertificates) {
  CertificateList certs;

  const base::Time now = base::Time::Now();
  const base::TimeDelta five_days = base::TimeDelta::FromDays(5);

  certs.push_back(scoped_refptr<X509Certificate>(NULL));
  certs.push_back(new X509Certificate(
      "expired", "expired",
      base::Time::UnixEpoch(), base::Time::UnixEpoch()));
  certs.push_back(new X509Certificate(
      "not yet valid", "not yet valid",
      base::Time::Max(), base::Time::Max()));
  certs.push_back(new X509Certificate(
      "older cert", "older cert",
      now - five_days, now + five_days));
  certs.push_back(scoped_refptr<X509Certificate>(NULL));
  certs.push_back(new X509Certificate(
      "newer cert", "newer cert",
      now - base::TimeDelta::FromDays(3), now + five_days));

  std::sort(certs.begin(), certs.end(), ClientCertSorter());

  ASSERT_TRUE(certs[0].get());
  EXPECT_EQ("newer cert", certs[0]->subject().common_name);
  ASSERT_TRUE(certs[1].get());
  EXPECT_EQ("older cert", certs[1]->subject().common_name);
  ASSERT_TRUE(certs[2].get());
  EXPECT_EQ("not yet valid", certs[2]->subject().common_name);
  ASSERT_TRUE(certs[3].get());
  EXPECT_EQ("expired", certs[3]->subject().common_name);
  ASSERT_FALSE(certs[4].get());
  ASSERT_FALSE(certs[5].get());
}

// This test creates a self-signed cert and a private key and then verifies the
// content of the certificate.
TEST(X509UtilTest, CreateKeyAndSelfSigned) {
  scoped_ptr<crypto::RSAPrivateKey> private_key;

  std::string der_cert;
  ASSERT_TRUE(x509_util::CreateKeyAndSelfSignedCert(
      "CN=subject",
      1,
      base::Time::Now(),
      base::Time::Now() + base::TimeDelta::FromDays(1),
      &private_key,
      &der_cert));

  ASSERT_TRUE(private_key.get());

  scoped_refptr<X509Certificate> cert(X509Certificate::CreateFromBytes(
      der_cert.data(), der_cert.size()));
  ASSERT_TRUE(cert.get());

  EXPECT_EQ("subject", cert->subject().GetDisplayName());
  EXPECT_FALSE(cert->HasExpired());
}

// This test creates a self-signed cert from a private key and then verifies the
// content of the certificate.
TEST(X509UtilTest, CreateSelfSigned) {
  const uint8 private_key_info[] = {
    0x30, 0x82, 0x02, 0x78, 0x02, 0x01, 0x00, 0x30,
    0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
    0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x04, 0x82,
    0x02, 0x62, 0x30, 0x82, 0x02, 0x5e, 0x02, 0x01,
    0x00, 0x02, 0x81, 0x81, 0x00, 0xb8, 0x7f, 0x2b,
    0x20, 0xdc, 0x7c, 0x9b, 0x0c, 0xdc, 0x51, 0x61,
    0x99, 0x0d, 0x36, 0x0f, 0xd4, 0x66, 0x88, 0x08,
    0x55, 0x84, 0xd5, 0x3a, 0xbf, 0x2b, 0xa4, 0x64,
    0x85, 0x7b, 0x0c, 0x04, 0x13, 0x3f, 0x8d, 0xf4,
    0xbc, 0x38, 0x0d, 0x49, 0xfe, 0x6b, 0xc4, 0x5a,
    0xb0, 0x40, 0x53, 0x3a, 0xd7, 0x66, 0x09, 0x0f,
    0x9e, 0x36, 0x74, 0x30, 0xda, 0x8a, 0x31, 0x4f,
    0x1f, 0x14, 0x50, 0xd7, 0xc7, 0x20, 0x94, 0x17,
    0xde, 0x4e, 0xb9, 0x57, 0x5e, 0x7e, 0x0a, 0xe5,
    0xb2, 0x65, 0x7a, 0x89, 0x4e, 0xb6, 0x47, 0xff,
    0x1c, 0xbd, 0xb7, 0x38, 0x13, 0xaf, 0x47, 0x85,
    0x84, 0x32, 0x33, 0xf3, 0x17, 0x49, 0xbf, 0xe9,
    0x96, 0xd0, 0xd6, 0x14, 0x6f, 0x13, 0x8d, 0xc5,
    0xfc, 0x2c, 0x72, 0xba, 0xac, 0xea, 0x7e, 0x18,
    0x53, 0x56, 0xa6, 0x83, 0xa2, 0xce, 0x93, 0x93,
    0xe7, 0x1f, 0x0f, 0xe6, 0x0f, 0x02, 0x03, 0x01,
    0x00, 0x01, 0x02, 0x81, 0x80, 0x03, 0x61, 0x89,
    0x37, 0xcb, 0xf2, 0x98, 0xa0, 0xce, 0xb4, 0xcb,
    0x16, 0x13, 0xf0, 0xe6, 0xaf, 0x5c, 0xc5, 0xa7,
    0x69, 0x71, 0xca, 0xba, 0x8d, 0xe0, 0x4d, 0xdd,
    0xed, 0xb8, 0x48, 0x8b, 0x16, 0x93, 0x36, 0x95,
    0xc2, 0x91, 0x40, 0x65, 0x17, 0xbd, 0x7f, 0xd6,
    0xad, 0x9e, 0x30, 0x28, 0x46, 0xe4, 0x3e, 0xcc,
    0x43, 0x78, 0xf9, 0xfe, 0x1f, 0x33, 0x23, 0x1e,
    0x31, 0x12, 0x9d, 0x3c, 0xa7, 0x08, 0x82, 0x7b,
    0x7d, 0x25, 0x4e, 0x5e, 0x19, 0xa8, 0x9b, 0xed,
    0x86, 0xb2, 0xcb, 0x3c, 0xfe, 0x4e, 0xa1, 0xfa,
    0x62, 0x87, 0x3a, 0x17, 0xf7, 0x60, 0xec, 0x38,
    0x29, 0xe8, 0x4f, 0x34, 0x9f, 0x76, 0x9d, 0xee,
    0xa3, 0xf6, 0x85, 0x6b, 0x84, 0x43, 0xc9, 0x1e,
    0x01, 0xff, 0xfd, 0xd0, 0x29, 0x4c, 0xfa, 0x8e,
    0x57, 0x0c, 0xc0, 0x71, 0xa5, 0xbb, 0x88, 0x46,
    0x29, 0x5c, 0xc0, 0x4f, 0x01, 0x02, 0x41, 0x00,
    0xf5, 0x83, 0xa4, 0x64, 0x4a, 0xf2, 0xdd, 0x8c,
    0x2c, 0xed, 0xa8, 0xd5, 0x60, 0x5a, 0xe4, 0xc7,
    0xcc, 0x61, 0xcd, 0x38, 0x42, 0x20, 0xd3, 0x82,
    0x18, 0xf2, 0x35, 0x00, 0x72, 0x2d, 0xf7, 0x89,
    0x80, 0x67, 0xb5, 0x93, 0x05, 0x5f, 0xdd, 0x42,
    0xba, 0x16, 0x1a, 0xea, 0x15, 0xc6, 0xf0, 0xb8,
    0x8c, 0xbc, 0xbf, 0x54, 0x9e, 0xf1, 0xc1, 0xb2,
    0xb3, 0x8b, 0xb6, 0x26, 0x02, 0x30, 0xc4, 0x81,
    0x02, 0x41, 0x00, 0xc0, 0x60, 0x62, 0x80, 0xe1,
    0x22, 0x78, 0xf6, 0x9d, 0x83, 0x18, 0xeb, 0x72,
    0x45, 0xd7, 0xc8, 0x01, 0x7f, 0xa9, 0xca, 0x8f,
    0x7d, 0xd6, 0xb8, 0x31, 0x2b, 0x84, 0x7f, 0x62,
    0xd9, 0xa9, 0x22, 0x17, 0x7d, 0x06, 0x35, 0x6c,
    0xf3, 0xc1, 0x94, 0x17, 0x85, 0x5a, 0xaf, 0x9c,
    0x5c, 0x09, 0x3c, 0xcf, 0x2f, 0x44, 0x9d, 0xb6,
    0x52, 0x68, 0x5f, 0xf9, 0x59, 0xc8, 0x84, 0x2b,
    0x39, 0x22, 0x8f, 0x02, 0x41, 0x00, 0xb2, 0x04,
    0xe2, 0x0e, 0x56, 0xca, 0x03, 0x1a, 0xc0, 0xf9,
    0x12, 0x92, 0xa5, 0x6b, 0x42, 0xb8, 0x1c, 0xda,
    0x4d, 0x93, 0x9d, 0x5f, 0x6f, 0xfd, 0xc5, 0x58,
    0xda, 0x55, 0x98, 0x74, 0xfc, 0x28, 0x17, 0x93,
    0x1b, 0x75, 0x9f, 0x50, 0x03, 0x7f, 0x7e, 0xae,
    0xc8, 0x95, 0x33, 0x75, 0x2c, 0xd6, 0xa4, 0x35,
    0xb8, 0x06, 0x03, 0xba, 0x08, 0x59, 0x2b, 0x17,
    0x02, 0xdc, 0x4c, 0x7a, 0x50, 0x01, 0x02, 0x41,
    0x00, 0x9d, 0xdb, 0x39, 0x59, 0x09, 0xe4, 0x30,
    0xa0, 0x24, 0xf5, 0xdb, 0x2f, 0xf0, 0x2f, 0xf1,
    0x75, 0x74, 0x0d, 0x5e, 0xb5, 0x11, 0x73, 0xb0,
    0x0a, 0xaa, 0x86, 0x4c, 0x0d, 0xff, 0x7e, 0x1d,
    0xb4, 0x14, 0xd4, 0x09, 0x91, 0x33, 0x5a, 0xfd,
    0xa0, 0x58, 0x80, 0x9b, 0xbe, 0x78, 0x2e, 0x69,
    0x82, 0x15, 0x7c, 0x72, 0xf0, 0x7b, 0x18, 0x39,
    0xff, 0x6e, 0xeb, 0xc6, 0x86, 0xf5, 0xb4, 0xc7,
    0x6f, 0x02, 0x41, 0x00, 0x8d, 0x1a, 0x37, 0x0f,
    0x76, 0xc4, 0x82, 0xfa, 0x5c, 0xc3, 0x79, 0x35,
    0x3e, 0x70, 0x8a, 0xbf, 0x27, 0x49, 0xb0, 0x99,
    0x63, 0xcb, 0x77, 0x5f, 0xa8, 0x82, 0x65, 0xf6,
    0x03, 0x52, 0x51, 0xf1, 0xae, 0x2e, 0x05, 0xb3,
    0xc6, 0xa4, 0x92, 0xd1, 0xce, 0x6c, 0x72, 0xfb,
    0x21, 0xb3, 0x02, 0x87, 0xe4, 0xfd, 0x61, 0xca,
    0x00, 0x42, 0x19, 0xf0, 0xda, 0x5a, 0x53, 0xe3,
    0xb1, 0xc5, 0x15, 0xf3
  };

  std::vector<uint8> input;
  input.resize(sizeof(private_key_info));
  memcpy(&input.front(), private_key_info, sizeof(private_key_info));

  scoped_ptr<crypto::RSAPrivateKey> private_key(
      crypto::RSAPrivateKey::CreateFromPrivateKeyInfo(input));
  ASSERT_TRUE(private_key.get());

  std::string der_cert;
  ASSERT_TRUE(x509_util::CreateSelfSignedCert(
      private_key.get(),
      x509_util::DIGEST_SHA1,
      "CN=subject",
      1,
      base::Time::Now(),
      base::Time::Now() + base::TimeDelta::FromDays(1),
      &der_cert));

  scoped_refptr<X509Certificate> cert =
      X509Certificate::CreateFromBytes(der_cert.data(), der_cert.size());
  ASSERT_TRUE(cert.get());

  EXPECT_EQ("subject", cert->subject().GetDisplayName());
  EXPECT_FALSE(cert->HasExpired());
}

}  // namespace x509_util

}  // namespace net
