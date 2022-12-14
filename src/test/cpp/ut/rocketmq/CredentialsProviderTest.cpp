/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "rocketmq/CredentialsProvider.h"
#include "MixAll.h"
#include "ghc/filesystem.hpp"
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/util/json_util.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

ROCKETMQ_NAMESPACE_BEGIN

class CredentialsProviderTest : public testing::Test {};

TEST_F(CredentialsProviderTest, testStaticCredentialsProvider) {
  std::string access_key("abc");
  std::string access_secret("def");
  StaticCredentialsProvider credentials_provider(access_key, access_secret);
  Credentials&& credentials = credentials_provider.getCredentials();
  ASSERT_EQ(credentials.accessKey(), access_key);
  ASSERT_EQ(credentials.accessSecret(), access_secret);
}

TEST_F(CredentialsProviderTest, testEnvironmentVariable) {
  const char* access_key = "abc";
  const char* access_secret = "def";

#ifdef _WIN32
  std::string env_access_key;
  env_access_key.append(EnvironmentVariablesCredentialsProvider::ENVIRONMENT_ACCESS_KEY);
  env_access_key.push_back('=');
  env_access_key.append(access_key);

  _putenv(env_access_key.c_str());

  std::string env_access_secret;
  env_access_secret.append(EnvironmentVariablesCredentialsProvider::ENVIRONMENT_ACCESS_SECRET);
  env_access_secret.push_back('=');
  env_access_secret.append(access_secret);
  _putenv(env_access_secret.c_str());
#else
  setenv(EnvironmentVariablesCredentialsProvider::ENVIRONMENT_ACCESS_KEY, access_key, 1);
  setenv(EnvironmentVariablesCredentialsProvider::ENVIRONMENT_ACCESS_SECRET, access_secret, 1);
#endif

  EnvironmentVariablesCredentialsProvider provider;
  const Credentials& credentials = provider.getCredentials();
  EXPECT_STREQ(access_key, credentials.accessKey().c_str());
  EXPECT_STREQ(access_secret, credentials.accessSecret().c_str());
}

ROCKETMQ_NAMESPACE_END